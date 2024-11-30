\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{fitthesispresn}[2020/09/14]

%Vychozi hodnoty parametru / Default values for parameters
\newif\ifczech \let\ifczech\iftrue
\newif\ifenglish \let\ifenglish\iffalse

%-----------------------------------------------------------------------------
% Parametry tridy (odkomentovani zakomentovanych na vlastni nebezpeci)
% Class parameters (uncommenting of commented one is only on your own risk)
%-----------------------------------------------------------------------------
\DeclareOption{english}{\let\ifenglish\iftrue\let\ifczech\iffalse}
\DeclareOption*{}
\ProcessOptions\relax
\LoadClass[10pt,xcolor=pdflatex,hyperref={unicode}]{beamer}

%-----------------------------------------------------------------------------
% Balicky / Packages
%-----------------------------------------------------------------------------
\ifenglish
  \PassOptionsToPackage{main=english}{babel}
\fi
\usepackage[czech,slovak,english]{babel}
\usepackage[utf8]{inputenc} %kodovani / encoding
\usepackage{newcent}
\usepackage{hyperref}
\usepackage{fancyvrb}
\usepackage{appendixnumberbeamer}
\usepackage{tikz}

% Volba loga dle zvoleneho jazyka / Logo selection according to the chosen language
\ifenglish
  \usetheme{FIT}
\else
  \usetheme[logocz]{FIT}
\fi

\ifczech
  \main@language{czech}
\else
  \main@language{english}
\fi

%-----------------------------------------------------------------------------
% Texty / Texts
%-----------------------------------------------------------------------------
\def\@supervisor@CS{Vedoucí: }
\def\@supervisor@EN{Supervisor: }

%-----------------------------------------------------------------------------
% Projectinfo - parametrizace sablony / template parameters
%-----------------------------------------------------------------------------
% Parametry / Parameters
\define@key{projinfo}{title}{\def\@title{#1}}
\define@key{projinfo}{title.footer}{\def\@title@f{#1}}
\define@key{projinfo}{author.name}{\def\@author@n{#1}}
\define@key{projinfo}{supervisor.name}{\def\@supervisor@n{#1}}
\define@key{projinfo}{supervisor.surname}{\def\@supervisor@s{#1}}
\define@key{projinfo}{member.name}{\def\@member@n{#1}}
\define@key{projinfo}{member.surname}{\def\@member@s{#1}}
\define@key{projinfo}{secmember.name}{\def\@secmember@n{#1}}
\define@key{projinfo}{secmember.surname}{\def\@secmember@s{#1}}
\define@key{projinfo}{thirdmember.name}{\def\@thirdmember@n{#1}}
\define@key{projinfo}{thirdmember.surname}{\def\@thirdmember@s{#1}}
\define@key{projinfo}{date}{\def\@date{#1}}

% Vychozi hodnoty / Default values
\setkeys{projinfo}{
  date=\today
}
    
\ifczech
  \setkeys{projinfo}{
      title={Název prezentace}, title.footer={Název prezentace},
      author.name=Jméno,
      supervisor.name=Jméno, supervisor.surname=Příjmení,
      member.name=Jméno, member.surname=Příjmení,
      secmember.name=Jméno, secmember.surname=Příjmení,
      thirdmember.name=Jméno, thirdmember.surname=Příjmení,
    }%
\fi

\def\projectinfo{\setkeys{projinfo}}

% Cele jmeno autora/vedouciho / Full name of author/supervisor
\def\@author{\@author@n}
\def\@supervisor{\@supervisor@n\ \@supervisor@s}
\def\@member{\@member@n\ \@member@s}
\def\@secmember{\@secmember@n\ \@secmember@s}
\def\@thirdmember{\@thirdmember@n\ \@thirdmember@s}


%-----------------------------------------------------------------------------
% Titulní stránka / Title page
%-----------------------------------------------------------------------------
\title[\@title@f]{\@title}
\author[]{\@author}

\institute[]{\footnotesize{\@supervisor@CS \@supervisor \\ \@member \\ \@secmember \\ \@thirdmember} \\}
\date{\@date}
