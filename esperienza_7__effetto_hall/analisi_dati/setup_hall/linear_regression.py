import ROOT as root
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
from scipy import rand

from uncertainties import ufloat
from os.path import join

BASE_PATH_DATA = '/Users/massimosotgia/Desktop/uni_at_DIFI/Lab2/esperienza_7__effetto_hall/dati'
BASE_PATH_FIG = '/Users/massimosotgia/Desktop/uni_at_DIFI/Lab2/esperienza_7__effetto_hall/fig'
SYSTEMS = ['keithley', 'current']

def get_vrangerr(range1,
                 err_percent=0.035,
                 partitions=8):
    return range1*err_percent*partitions
    
def get_irangerr(meas):
    return meas * 0.000001 * 500 + 0.000001 * 400 * 0.1

class linear_regression:
    '''
    Regressione lineare

    Analisi della regrassione lineare per l'analisi di alcune componenti circuitali per E7

    Args:
    - filename: POSIX filename (only filename (assuming path is ../dati/)
    - optional: tension/current, keycodes to use different error propagation in the evaluation for parameters. 
    '''
    
    def __init__(self, 
                 filename=None, 
                 figsize=(800,600), 
                 atlasstyle=True, tsize=28,
                 system=None,
                 modelfnc=None,
                 subfig_label=None,
                 xlabel=None,
                 ylabel=None):
        
        self.setparameter: bool = False
        self.tsize = tsize
        
        if filename==None:
            raise Warning(f'Not a file {filename=}')
        else:
            self.__file__ = np.loadtxt(join(BASE_PATH_DATA, filename), dtype=np.float64) # array of Ncolumns x Nrows

        if atlasstyle:
            root.gInterpreter.AddIncludePath('/usr/local/include')
            root.gSystem.Load('/usr/local/lib/libErrorAnalysis')
            root.gSystem.Load('/usr/local/lib/libLabTools')
            root.gInterpreter.ProcessLine('''
            #include<LabPlot.h>
            #include<LabStat.h>
            ''')
            
            from ROOT import set_atlas_style
            set_atlas_style(self.tsize)
        
        if modelfnc == None:
            self.__modelfnc__ = '[0]*x + [1]' ## [0] is coefficient, [1] is offset
        else: self.__modelfnc__ = modelfnc

        self.__figure__ = root.TCanvas('__figure__'.format(rand), '', figsize[0], figsize[1])
        self.__graph__ = root.TGraphErrors()
        self.__model__ = root.TF1('__model__', self.__modelfnc__)
        self.__legend__ = root.TLegend(0.2, 0.65, 0.5, 0.9)
        self.__legend__.SetFillColorAlpha(0, 0.75)
        self.__legend__.SetTextSize(self.tsize-2)
        
        '''
        read file and fill __graph__
        '''
        
        self.readfile_asarray(system=system)
        
        if system=='keithley':
            self.__graph__.SetTitle(';Input voltage V_{in} [V];Output voltage V_{out} [V]')
        elif system=='current':
            self.__graph__.SetTitle(';Input voltage V_{-} - V_{+} [V];Current [mA]')
        else:
            if xlabel==None or ylabel==None:
                raise Warning('set x/y labels...')
            else:
                self.__graph__.SetTitle(';{};{}'.format(xlabel, ylabel))
        
        self.__subfiglabel__ = subfig_label
    
    def get_graph(self):
        return self.__graph__
    
    def get_figure(self):
        return self.__figure__
    
    def get_model(self):
        return self.__model__
    
    def set_model_parameters(self, p0=None, p1=None):
        '''
        Args:
         - if `keithley`:
            - p0: correlation parameter (gain)
            - p1: offset
         - if `current`:
            - p0: 1/R5*(R4/R3 || R2/R1)
            - p1 offset
        '''
        self.__model__.SetParameters(p0, p1)
        self.setparameter = True
        
    def get_correlation_offset_parameter(self):
        '''
        Returns: par0, par1 ( = correlation parameter, offset )
        '''
        
        par0 = (self.__model__.GetParameter(0), self.__model__.GetParError(0))
        par1 = (self.__model__.GetParameter(1), self.__model__.GetParError(1))
        print('correlation parameter: {:.3uS} [arb. u.]\noffset: {:.3uS} [coherent units with y-axis value]'
              .format(ufloat(par0[0], par0[1]), ufloat(par1[0], par1[1])))
        
        return par0, par1
    
    def showfig(self,fmt: 'filename for figure (saved in ../fig/'=None, 
                subfig_label=None, optional_label=None, 
                subfigposition=(.8,.8), statposition=(.7,.3)):
        '''
        Plot __graph__ obj in __figure__ frame
        
        Args:
         - fmt: filename for figure saved in ../fig/
         - subfig_label: optional, used for subfigures if any
         - optional_label: italic text in legend header
        '''
        
        if optional_label == None:
            self.__optlabel__ = 'Preliminary'
        else: 
            self.__optlabel__ = optional_label
        
        if self.__subfiglabel__ == None:
            self.__subfiglabel__ = subfig_label
        
        self.__figure__.cd()
        self.__graph__.Draw('AP')
        if not self.setparameter:
            raise Warning('model parameters not set!')
        else:
            self.__graph__.Fit('__model__')
            stat_txt = root.TLatex()
            stat_txt.SetTextSize(self.tsize-5)
            stat_txt.DrawLatexNDC(statposition[0], statposition[1], '#splitline{{#chi^{{2}}/NDF   {:.3}/{}}}{{Prob.(#chi^{{2}})   {:.3}}}'
                                  .format(self.__model__.GetChisquare(),self.__model__.GetNDF(),self.__model__.GetProb()))
            print('model results chi2/ndf (prob): {}/{} ({})'
                  .format(self.__model__.GetChisquare(),self.__model__.GetNDF(),self.__model__.GetProb()))
        
        if self.__subfiglabel__ != None:
            aux_txt = root.TLatex()
            aux_txt.SetTextSize(self.tsize)
            aux_txt.DrawLatexNDC(subfigposition[0], subfigposition[1], '#bf{{{}}}'.format(self.__subfiglabel__))
        
        
        self.__legend__.SetHeader('#bf{{E7/R3 Setup}} #it{{{}}}'.format(self.__optlabel__))
        self.__legend__.AddEntry(self.__graph__, 'Data', 'LPE')
        self.__legend__.Draw()
        
        self.__figure__.Draw()
        if fmt is None:
            self.__figure__.Print('fFigure.pdf')
        else:
            self.__figure__.Print(join(BASE_PATH_FIG, '{}.pdf'.format(fmt)))
    
    def fit_model(self,fit_range=None):
        if not self.setparameter:
            raise Warning('model parameters not set!')
        else:
            self.__graph__.Fit('__model__')
            print('model results chi2/ndf (prob): {}/{} ({})'
                  .format(self.__model__.GetChisquare(),self.__model__.GetNDF(),self.__model__.GetProb()))
            mod = self.__model__
            p0 = (mod.GetParameter(0), mod.GetParError(0))
            p1 = (mod.GetParameter(1), mod.GetParError(1))
            
            print('correlation parameter: {:.3uS} [arb. u.]\noffset: {:.3uS} [coherent units with y-axis value]'
                  .format(ufloat(p0[0], p0[1]), ufloat(p1[0], p1[1])))
            
            return (p0,p1)
        return None    
        
    def readfile_asarray(self, system=None):
        
        if system not in SYSTEMS:
            raise Warning(f'{system=} is not defined! Use {SYSTEMS}')
        elif system == 'keithley':
            '''
            keithley: using function get_vrangerr,
             - file is in format: vin fsvin vout fsvout
             - gain is vout/vin.
             - offset should be zero
            '''
            for idx, point in enumerate(self.__file__):
                # analize line[idx] of array numpy
                self.__graph__.SetPoint(idx, point[0], point[2])
                self.__graph__.SetPointError(idx, 
                                             get_vrangerr(range1=point[1]),
                                             get_vrangerr(range1=point[3]))
                
        else:
            '''
            amperometre: using function get_irangerr,
             - file is in format: vin fsvin vout fsvout
             - 1/R5 should be 1/500.
             - offset should be zero, or dictated by R2/R1==R4/R3
            '''
            for idx, point in enumerate(self.__file__):
                # analize line[idx] of array numpy
                self.__graph__.SetPoint(idx, point[0], point[2]*1000)
                self.__graph__.SetPointError(idx, 
                                             get_vrangerr(range1=point[1]),
                                             get_irangerr(meas=point[2])*1000)
        

