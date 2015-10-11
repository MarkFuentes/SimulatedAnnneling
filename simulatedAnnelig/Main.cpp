#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<iostream>
#
//Estructuras de datos
/*
Instancia guarda las coordenadas registradas
en un archivo llamado An33.txt.
Capacidad guarda la máxima capacidad por cada ruta
Demanda tiene el valor que pide cada cliente
CoordX es la posicion del cliente en el eje x
CoordY es la posicion del cliente en el eje y
*/
typedef struct 
{
	int capacidad; 
	int *coordX = (int*)malloc(1 * sizeof(int));
	int *coordY = (int*)malloc(1 * sizeof(int));
	int *demanda = (int*)malloc(1 * sizeof(int)); 
}instancia;
/*
Solucion guarda una serie de clientes registrados 
en un archivo llamado rutas.txt, se crea un conjunto de clientes 
para formar rutas y cada ruta esta delimitada por limites inferiores
y superiores. Cada elemento de la descripción se guarda en ésta estructura 
en forma de vector
*/
typedef struct 
{
	int *cliente = (int*)malloc(1 * sizeof(int));    
	int *limiteSuperior = (int*)malloc(1 * sizeof(int)); 
	int *limiteInferior = (int*)malloc(1 * sizeof(int)); 
}solucion;
//Porototipo de funciones
void leeArchivos();
void distancias();
double calculaCostoSolucion(solucion *);
void imprimeSolucion(solucion *);
//Variables
/*Variables de tipo estructura*/
instancia _instancia;
instancia *_ptrInstancia = &_instancia;
solucion _solucion;
solucion *_ptrSolucion = &_solucion;
/*Variables tipo de dato primitivo*/
int numClientes = 0; //cantidad de clientes registrados en el archivo
int totRutas = 0;	//numero de vehiculos registradas en el archivo, posteriormente se recalcula con limitBuilder
const char *nomInstancia = { "An33.txt" };
const char *nomSolucion = { "rutas.txt" };
double *mDistancias = (double*)malloc(1 * sizeof(double));
//Implementación de funciones
void leeArchivos() {
	FILE *archInstancia;
	FILE *archSolucion;
	int iCounter, iValor;
	int elemenTracker = 1; // cuenta todos los elementos registrados en un archivo, número de clientes
	int *_ptrTemp; //Puntero que ayuda a la reasignación de memoria
	if ((archInstancia = fopen(nomInstancia, "rt")) == NULL)
	{
		printf("No existe el archivo de datos");
	}
	else
	{
		fscanf(archInstancia, " %d\t ", &_instancia.capacidad);
		do 
		{
			fscanf(archInstancia, " %d\t ", &_instancia.coordX[elemenTracker - 1]);
			fscanf(archInstancia, " %d\t ", &_instancia.coordY[elemenTracker - 1]);
			fscanf(archInstancia, " %d\t ", &_instancia.demanda[elemenTracker - 1]);
			elemenTracker += 1;
			_ptrTemp = (int*)realloc(_ptrInstancia->coordX, (elemenTracker * sizeof(int)));
			_ptrInstancia->coordX = _ptrTemp;
			_ptrTemp = (int*)realloc(_ptrInstancia->coordY, (elemenTracker * sizeof(int)));
			_ptrInstancia->coordY = _ptrTemp;
			_ptrTemp = (int*)realloc(_ptrInstancia->demanda, (elemenTracker * sizeof(int)));
			_ptrInstancia->demanda = _ptrTemp;
		} 
		while (!feof(archInstancia));
		elemenTracker -= 1;
		fclose(archInstancia);
	}
	numClientes = elemenTracker;
	if ((archSolucion = fopen(nomSolucion, "rt")) == NULL) 
	{
		printf("No existe el archivo de rutas inicial");
	}
	else 
	{
		fscanf(archSolucion, "%d\t", &totRutas);
		elemenTracker = 1; //ReInit for reallocate solution arrays
		for (iCounter = 0; iCounter < numClientes; iCounter ++)
		{
			fscanf(archSolucion, "%d\t", &iValor);
			if (iValor != 0)
			{
				_ptrSolucion->cliente[iCounter] = iValor;
				elemenTracker += 1;
				_ptrTemp = (int*)realloc(_ptrSolucion->cliente, (elemenTracker * sizeof(int)));
				_ptrSolucion->cliente = _ptrTemp;
			}
		}
		fscanf(archSolucion, "%d\t", &iValor);
		elemenTracker = 1; //ReInit for reallocate solution arrays
		for (iCounter = 0; iCounter < totRutas; iCounter ++)
		{
			fscanf(archSolucion, "%d\t", &iValor);
			_ptrSolucion->limiteInferior[iCounter] = iValor;
			elemenTracker += 1;
			_ptrTemp = (int*)realloc(_ptrSolucion->limiteInferior, (elemenTracker * sizeof(int)));
			_ptrSolucion->limiteInferior = _ptrTemp;
		}
		fscanf(archSolucion, "%d\t", &iValor);
		elemenTracker = 1; //ReInit for reallocate solution arrays
		for (iCounter = 0; iCounter < totRutas; iCounter ++)
		{
			fscanf(archSolucion, "%d\t", &iValor);
			_ptrSolucion->limiteSuperior[iCounter] = iValor;
			elemenTracker += 1;
			_ptrTemp = (int*)realloc(_ptrSolucion->limiteSuperior, (elemenTracker * sizeof(int)));
			_ptrSolucion->limiteSuperior = _ptrTemp;
		}
	}
	fclose(archSolucion);
}
void imprimeSolucion(solucion *p)
{
	int iCounter = 0;
	std::cout << "Total de rutas:" << totRutas << std::endl;
	std::cout << "Numero de clientes:" << numClientes << std::endl;
	for (iCounter = 0; iCounter < numClientes - 1; iCounter++)
	{
		std::cout << "Ruta:" << p->cliente[iCounter] << std::endl;
	}
	std::cout << "Limites inferiores" << std::endl;
	for (iCounter = 0; iCounter < totRutas; iCounter++)
	{
		std::cout << "Li:" << p->limiteInferior[iCounter] << std::endl;
	}
	std::cout << "Limites superiores" << std::endl;
	for (iCounter = 0; iCounter < totRutas; iCounter++)
	{
		std::cout << "Ls:" << p->limiteSuperior[iCounter] << std::endl;
	}
}
void distancias() 
{
	int iCounter = 0;
	int jCounter = 0;
	float xx = 0.0F, yy = 0.0F;
	double *_ptrTemp = (double*)realloc(mDistancias, ((numClientes * numClientes) * sizeof(double)));
	mDistancias = _ptrTemp;
	for (iCounter = 0; iCounter < numClientes; iCounter ++)
	{
		for (jCounter = (iCounter + 1); jCounter < numClientes; jCounter ++)
		{
			xx = (float)(_ptrInstancia->coordX[iCounter] - _ptrInstancia->coordX[jCounter])
				*(_ptrInstancia->coordX[iCounter] - _ptrInstancia->coordX[jCounter]);
			yy = (float)(_ptrInstancia->coordY[iCounter] - _ptrInstancia->coordY[jCounter])
				*(_ptrInstancia->coordY[iCounter] - _ptrInstancia->coordY[jCounter]);
			mDistancias[(numClientes * jCounter) + iCounter] = mDistancias[((numClientes * iCounter) + jCounter)] = sqrt(xx + yy);
		}
	}
}
double calculaCostoSolucion(solucion *p)
{
	int iCounter = 0;
	int jCounter = 0;
	double costoIntraRuta = 0;
	double costoInterRuta = 0;
	for (iCounter = 0; iCounter < totRutas; iCounter ++)
	{
		costoIntraRuta = 0;
		costoIntraRuta += mDistancias[(numClientes * 0)  + p->cliente[p->limiteInferior[iCounter]]]; //Distancia Primer cliente
		for (jCounter = p->limiteInferior[iCounter]; jCounter < p->limiteSuperior[iCounter]; jCounter ++)
		{
			costoIntraRuta += mDistancias[(numClientes * p->cliente[jCounter]) + p->cliente[jCounter + 1]];
		}
		costoIntraRuta += mDistancias[(numClientes * p->cliente[jCounter]) + 0]; //distancia del ultimo cliente al deposito
		std::cout << costoIntraRuta << std::endl;
		costoInterRuta += costoIntraRuta;
	}
	return (costoInterRuta);
}
void main()
{
	leeArchivos();
	distancias();
	calculaCostoSolucion(_ptrSolucion);
	int pause = 0;
	std::cin >> pause;
}
