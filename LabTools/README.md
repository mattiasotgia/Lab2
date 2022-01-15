# LabTools

Inside [LabTools](LabTools/) there is the full definition and implementation for the LabTools c++ library. It needs to be compiled and if used in ROOT needs to be included in the `rootlogon.C` root file. 

## How to install

inside `LabTools` folder go to `build/` (`cd build`).
Then do the following
```bash
$ cmake ..
$ make
$ sudo make install
```
Then go to $HOME and create (if non existing!) the `.rootrc` file by doing
```bash
$ touch .rootrc
```
and add the following line to `.rootrc`:
```
Rint.Logon: ~/rootlogon.C
```

Finally, in your $HOME directory create a file named `rootlogon.C` by doing
```bash
$ touch rootlogon.C
```
and modify this file adding the following lines
```cpp
{
    gInterpreter->AddIncludePath("/usr/local/include");
    gSystem->Load("/usr/local/lib/libLabTools");
}
```

Now to load the Library in any program you'll need to
```cpp
#include<LabTools.h>
```

If using ROOT macros, no other compiler instruction is needed.
To use this library in compiled c/c++ programs you'll need to do the following: 
```bash
$ g++ file_name.cpp -o exe_name -I/usr/local/include -L/usr/local/lib/ -lLabTools `root-config --glibs --cflags`
```
where `` `root-config --glibs --cflags` `` is only needed if the code calls for other ROOT specific functions.

## Usage
### Namespace `base`
Some standard methods.

`bool base::isIn(_container _C, const _Ty &_Val)` check if given value/string (`class _Ty` object) is inside a container (`class _container`:`enum` or `union` type). 

### Namespace `logs`
Logging passed to stdout. This is not a logger!

`void logs::print_mmsg(std::string mmsg)`: print `mmsg` to stdout.

`void logs::print_stat(TF1 *_f)`: print <!-- $\chi^2/\text{ndf}$ --> <img style="transform: translateY(0.1em); background: white;" src="https://render.githubusercontent.com/render/math?math=%5Cchi%5E2%2F%5Ctext%7Bndf%7D"> and <!-- $\text{prob}(\chi^2)$ --> <img style="transform: translateY(0.1em); background: white;" src="https://render.githubusercontent.com/render/math?math=%5Ctext%7Bprob%7D(%5Cchi%5E2)"> to stdout. 

### Namespace `stattools`
Some useful and recurring statistical-computation functions.

`double stattools::max_to_stat(double value)`: divide any given number by <!-- $\sqrt{3}$ --> <img style="transform: translateY(0.1em); background: white;" src="https://render.githubusercontent.com/render/math?math=%5Csqrt%7B3%7D">.

`std::string stattools::compatible(double G1, double errG1, double G2, double errG2)`: check <!-- $3 \sigma$ --> <img style="transform: translateY(0.1em); background: white;" src="https://render.githubusercontent.com/render/math?math=3%20%5Csigma"> compatibility between G1 and G2, given their error.

`double stattools::getbestvalue(double G1, double G2, double errG1, double errG2)`: compute error-weighted mean value.

`double stattools::getbestvalueerr(double errG1, double errG2)`: compute error-weighted mean error.

### Namespace `graphset`

`void graphset::set_TGraphAxis(_TObj *g, std::string ytitle, float offset, std::string xtitle)`: Automatically sets-up the `_TObj` graph axis, and combined with the following methods, is pretty useful for creating residuals for current graph.

`void graphset::set_ResidualsAxis(_TObj *rg, std::string xtitle, float offset, std::string ytitle)`: sets-up residual graph axis.

`struct graphset::padtypes`: struct that contain two objects: `graphset::padtypes::Graph` and `graphset::padtypes::Residuals` useful for passin arguments.

`void graphset::setgraphsize(graphset::padtypes g, bool logx, bool logy, bool drawresiduals)`: set automatic graph size, logarithmic axis if needed, and turn on and off residual graph.

`void graphset::setcanvas(_TObj *c1, int nx, int ny, float m_left, float m_right, float m_bottom, float m_top)`: auto set canvas size. All parameters are altready set, so basic usage is by passing only `_TObj` as argument.

`void graphset::fillresiduals(_TObj *g, _TFObj *g_fit, TGraphErrors *r)`: fill residuals for any `_TObj` using `_TFObj` fit function.

`void graphset::setmarker()`: set marker and full line color and get matching both.