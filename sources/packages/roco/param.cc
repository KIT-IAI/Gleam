#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <iostream.h>    wj!
// #include <fstream.h>     wj!
#include <iostream>
#include <fstream>

#include "param.h"

#define DEBUG 0

#ifdef BS_DOS
#define dcout if(DEBUG) cout
#else
#define dcout if(DEBUG) std::cout
#endif /* BS_DOS */



// ************************************************************* //
// Zuweisungs-Operator                                           //
// ************************************************************* //

param& param::operator= (const param &p)
{
  if (this != &p)                       // Selbstzuweisung verhindern
  {
	 if (_name)
   	free (_name);
		
	 _name = strdup(p._name);             // Namen kopieren
	 _n = p._n;
	 if (mliste)
   	delete [] mliste;
	 mliste = new merkmal[_n];            // merkmalListe kopieren
	 for (int i = 0; i<_n; i++)
   	mliste[i] = p.mliste[i];
  }
  return *this;
}


// ************************************************************* //
// Parameter "name" hinzufügen                                   //
// ************************************************************* //

int paramListe::addparam (char *name)
{
  param *pliste1 = new param[_n+1];     // um ein Element größere Liste anlegen

  for (int i = 0; i < _n; i++)          // alten Inhalt dareinkopieren
    pliste1[i] = pliste[i];

  pliste1[_n].setname(name);
  
  if (pliste)
    delete [] pliste;
  pliste = pliste1;
  _n++;

  return _n-1;                          // Index des letzten Parameters zurückgeben
}


// ************************************************************* //
// auf einen Parameter in der Liste zugreifen                    //
// ************************************************************* //

param& paramListe::operator[] (int i)
{
  if ((i < 0) || (i >= _n))
  {
#ifdef BS_DOS
    cerr << "index=" << i << endl;
    cerr << "Class paramListe: wrong index." << endl;
#else
    std::cerr << "index=" << i << std::endl;
    std::cerr << "Class paramListe: wrong index." << std::endl;
#endif /* BS_DOS */
    exit(1);
  }

  return pliste[i];
}


// ************************************************************* //
// Inifile parsen                                                //
//   ininame: Name der Datei                                     //
// ************************************************************* //

int parse(char *ininame, paramListe &params, merkmalListe &props)
{
	const int BUFSIZE=512;
	char buffer[BUFSIZE];
	char *token;
	char *tmp;
	char *value;
	int line;
	
  // Datei öffnen
#ifdef BS_DOS
	ifstream file(ininame);
#else
	std::ifstream file(ininame);
#endif /* BS_DOS */
	if(!file) return -1;
	
  // Zeilenweise einlesen bis zum Dateiende
	for(line=1;!file.eof();line++)
	{
		file.getline(buffer,BUFSIZE);
#ifdef BS_DOS
		dcout<<buffer<<endl;
#else
		dcout<<buffer<<std::endl;
#endif /* BS_DOS */

    // Kommentare und Leerzeilen
		if(strchr("#\0",*(token=buffer+strspn(buffer," \t")))) continue;
		
		if(*(tmp=token+strcspn(token," \t="))!='\0')
		{
#ifdef BS_DOS
			dcout<<"a"<<endl;
#else
			dcout<<"a"<<std::endl;
#endif /* BS_DOS */
			*tmp='\0';
			tmp++;
		}

    // Parameterdefinitionen
		if(strcmp(token,"def")==0)
		{
#ifdef BS_DOS
			dcout<<"Schlüsselwort def wurde gefunden!"<<endl;
#else
			dcout<<"Schlüsselwort def wurde gefunden!"<<std::endl;
#endif /* BS_DOS */
			if(strchr("#\0",*(value=tmp+strspn(tmp," \t=")))) goto error;
			if(*(tmp=value+strcspn(value," \t"))!='\0')
			{
				*tmp='\0';
				tmp++;
			}
			int np=params.addparam(value);
			
			token=tmp+strspn(tmp," \t");
			if(*(tmp=token+strcspn(token," \t"))!='\0')
			{
				*tmp='\0';
				tmp++;
			}
			if(strcmp(token,"with") || !strchr("#\0",*(tmp+strspn(tmp," \t"))))
#ifdef BS_DOS
				cerr<<"In "<<ininame<<" Zeile "<<line<<":"<<endl
					<<"Korrekte Syntax lautet \"def <Parameter> with\""<<endl;
#else
				std::cerr<<"In "<<ininame<<" Zeile "<<line<<":"<<std::endl
					<<"Korrekte Syntax lautet \"def <Parameter> with\""<<std::endl;
#endif /* BS_DOS */
			do
			{
				file.getline(buffer,BUFSIZE);
				line++;
				
				if(strchr("#\0",*(token=buffer+strspn(buffer," \t")))) continue;
				if(*(tmp=token+strcspn(token," \t="))!='\0')
				{
					*tmp='\0';
					tmp++;
				}
				
				if(strncmp(token,"end",3)==0) break;
				
				if(strchr("#\0",*(value=tmp+strspn(tmp," \t=")))) goto error;
				for(tmp=value+strcspn(value,"#")-1;strchr(" \t",*tmp);tmp--);
				*(++tmp)='\0';
				
				params[np].addmerkmal(token,value);
			}
			while(!file.eof());
		}
		else if(strcmp(token,"startcomplex")==0)
		{
#ifdef BS_DOS
			dcout<<"b"<<endl;
#else
			dcout<<"b"<<std::endl;
#endif /* BS_DOS */
			int vertex = 0;
			for(vertex=0;!file.eof();vertex++)
			{
			  char name[20];                                  /* vorher 10, wj */
				sprintf(name,"vertex%d",vertex);
				file.getline(buffer,BUFSIZE);
				line++;
				
				if(!(token=strtok(buffer,",; ")) || (token[0]=='#')) continue;
				if(strcmp(token,"end")==0) break;
				buffer[0]='\0';
				do
				{
				  if ((strlen(buffer)+strlen(token)+1) < BUFSIZE)    /* wj */
					sprintf(buffer,"%s %s",buffer,token);
				}
				while((token=strtok(NULL,",; ")) && (token[0]!='#'));
				props.addmerkmal(name,buffer);
			}
			props.addmerkmal("nr_of_complex_points",vertex);
		}
		else
		{
#ifdef BS_DOS
			dcout<<"c"<<endl;
#else
			dcout<<"c"<<std::endl;
#endif /* BS_DOS */
			if(strchr("#\0",*(value=tmp+strspn(tmp," \t=")))) goto error;
			for(tmp=value+strcspn(value,"#")-1;strchr(" \t",*tmp);tmp--);
			*(++tmp)='\0';

			props.addmerkmal(token,value);
		}	
	}
	return 0;

error:
#ifdef BS_DOS
	cerr<<"In "<<ininame<<" Zeile "<<line<<":"<<endl
		<<"Syntaxfehler!"<<endl;
#else
	std::cerr<<"In "<<ininame<<" Zeile "<<line<<":"<<std::endl
		<<"Syntaxfehler!"<<std::endl;
#endif /* BS_DOS */
	exit(1);
}

		
void ReadIni (char *OptimizationIniName, char *FunctionIniName, paramListe &pl, merkmalListe &ml) // Einlesen des Parametersatzes vom user.ini
{
	if(parse(OptimizationIniName, pl, ml))
#ifdef BS_DOS
		cerr<<OptimizationIniName<<" nicht gefunden. Verwende Standardeinstellungen."<<endl;
#else
		std::cerr<<OptimizationIniName<<" nicht gefunden. Verwende Standardeinstellungen."<<std::endl;
#endif /* BS_DOS */
	if(parse(FunctionIniName, pl, ml))
	{
#ifdef BS_DOS
		cerr<<FunctionIniName<<" nicht gefunden."<<endl;
#else
		std::cerr<<FunctionIniName<<" nicht gefunden."<<std::endl;
#endif /* BS_DOS */
		exit(1);
	}
}
