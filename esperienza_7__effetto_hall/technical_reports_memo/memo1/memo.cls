\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{labmemo}[2022/03/05 Lab memo and technical reports]

%---- Options

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{revtex4-2}}
\ProcessOptions\relax

\LoadClass[
    varvw, 
    fleqn,
    amsmath, 
    amssymb, 
    a4paper,
    superscriptaddress,
    altaffilletter,
    rmp,
    raggedfooter
]{revtex4-2}


%---- Packages

\RequirePackage[T1]{fontenc}
\RequirePackage[utf8]{inputenc}
\RequirePackage[italian]{babel}

\RequirePackage{newtxtext, newtxmath}
% \renewcommand{\normalfont}{\sffamily}

\RequirePackage{hyperref}% add hypertext capabilities

\renewcommand\UrlFont{\color{black!50!blue}}
\hypersetup{
    colorlinks, urlcolor=., bookmarks,
    citecolor = black!50!blue,
    linkcolor = black!50!blue,
    urlcolor = black!50!blue,
    anchorcolor = black!50!blue
}

\RequirePackage{esint}
\RequirePackage{graphicx}% Include figure files

\RequirePackage{physics}
\RequirePackage{siunitx}

\sisetup{
    % per-mode=symbol,
    % separate-uncertainty=true,
    round-mode=uncertainty,
    % exponent-mode = scientific
}

\RequirePackage{background}
\SetBgColor{gray}
\SetBgAngle{90}
\SetBgScale{2}
\SetBgVshift{0.27\textwidth}
\SetBgContents{laboratorio2: Memo (dated \today)}



%% revtex4-2 bug-fix ----------------------------
\def\andname{e}
%--------------------
\let\it@comma@def\active@comma