#include <iostream>
#include <string>
#include <iomanip> //header para convertir float a 2 cifras
#include <limits>
#include <fstream>
#include <filesystem>
#include <vector>
#include <ctime>         //libreria para manejar hora
#include <sstream>       //libreria para manejar hora
#include <unordered_set> // almacena elementos únicos sin orden específico,
#include "Textable.h"
// Directivas Condicionales
#ifdef _WIN32
    // Código solo para Windows
    #include <windows.h>
    void limpiarPantalla() {
        system("cls");
    }
#else
    // Código para Linux/Mac
    #include <unistd.h>
    void limpiarPantalla() {
        system("clear");
    }
#endif
//**************************************************************************************************************************//
//-ESTRUCTURAS--------------------------------------------------------------------------------------------------------------//
//**************************************************************************************************************************//
using namespace std;
struct Conductor
{
    string dui;
    string nombre;
    string telefono;
    string direccion;
};

struct Vehiculo
{
    string placa;
    string marca;
    string modelo;
    int year;
    string categoria; // "Ejecutiva" o "Tradicional"
    string estado;    // "Disponible" o "En ruta"
    Conductor conductor;
};

struct Transaction
{
    string placa;
    string conductor;
    string origenDestino;
    float tarifa;
    string fechaViaje;
    string hora;
};

//**************************************************************************************************************************//
//-FUNCIONES----------------------------------------------------------------------------------------------------------------//
//**************************************************************************************************************************//

// la funcion GOTOXY cambia de a Linux y Windows(en este caso se esta utilizando la funcion para Linux)
void gotoxy(int x, int y)
{
#ifdef _WIN32 // funcion para Windows
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << y << ";" << x << "H"; // Funcion para linux
#endif
}

std::string obtenerHoraActual()
{ // obtiene la hora actual ya convertida a string
    std::time_t tiempo = std::time(nullptr);
    std::tm *horaLocal = std::localtime(&tiempo);

    std::ostringstream oss;
    oss << std::put_time(horaLocal, "%H:%M:%S");
    return oss.str();
}

void guardarVehiculo(const vector<Vehiculo> &taxis, const string &nombreArchivo)
{
    ofstream archivo(nombreArchivo);
    for (const auto &taxi : taxis)
    {
        archivo << "# Vehiculo" << endl
                << taxi.placa << endl
                << taxi.marca << endl
                << taxi.modelo << endl
                << taxi.year << endl
                << taxi.categoria << endl
                << taxi.estado << endl
                << "# Conductor" << endl
                << taxi.conductor.dui << endl
                << taxi.conductor.nombre << endl
                << taxi.conductor.telefono << endl
                << taxi.conductor.direccion << endl
                << endl;
    }
    archivo.close();
}

void guardarViaje(const vector<Transaction> &viajes, const string &nombreArchivo)
{
    ofstream archivo(nombreArchivo);
    for (const auto &viaje : viajes)
    {
        archivo << "# Transaccion Numero : " << endl
                << viaje.placa << ","
                << viaje.conductor << ","
                << viaje.origenDestino << ","
                << viaje.tarifa << ","
                << viaje.fechaViaje << ","
                << viaje.hora << endl
                << endl;
    }
    archivo.close();
}

void cargarFlotaTaxis(vector<Vehiculo> &listaTaxis, const string &nombreArchivo)
{
    ifstream archivo(nombreArchivo);
    Vehiculo taxi;

    string linea; // para almacenar temporalmente cada línea leída

    while (getline(archivo, linea))
    {
        if (linea == "# Vehiculo")
        {
            if (!getline(archivo, taxi.placa))
                break;
            if (!getline(archivo, taxi.marca))
                break;
            if (!getline(archivo, taxi.modelo))
                break;
            string yearStr; // Variable Temporal para cargar el año del vehiculo como int
            if (!getline(archivo, yearStr))
                break;
            taxi.year = stoi(yearStr);
            if (!getline(archivo, taxi.categoria))
                break;
            if (!getline(archivo, taxi.estado))
                break;

            // Verificar que sigue el conductor
            if (!getline(archivo, linea) || linea != "# Conductor")
            {
                cerr << "Error: Se esperaba # Conductor" << endl;
                break;
            }
            // Leer conductor
            if (!getline(archivo, taxi.conductor.dui))
                break;
            if (!getline(archivo, taxi.conductor.nombre))
                break;
            if (!getline(archivo, taxi.conductor.telefono))
                break;
            if (!getline(archivo, taxi.conductor.direccion))
                break;

            listaTaxis.push_back(taxi);
        }
    }

    archivo.close();
}

void cargarTransacciones(vector<Transaction> &viajes, const string &nombreArchivo)
{
    ifstream archivo(nombreArchivo);
    Vehiculo taxi;

    string linea;
    while (getline(archivo, linea))
    {
        // Saltar líneas que empiezan con '#'
        if (linea.empty() || linea[0] == '#')
            continue;

        // Crear objeto y parsear la línea CSV
        Transaction viaje;
        stringstream lector(linea);
        string campo;

        getline(lector, viaje.placa, ',');
        getline(lector, viaje.conductor, ',');
        getline(lector, viaje.origenDestino, ',');
        getline(lector, campo, ',');
        viaje.tarifa = stof(campo); // convertir a float
        getline(lector, viaje.fechaViaje, ',');
        getline(lector, viaje.hora);

        viajes.push_back(viaje);
    }

    archivo.close();
}

bool validarTaxiNuevo(const vector<Vehiculo> &listaTaxis, const string &placa, int year)
{

    // Validar si placa es Repetida
    for (const Vehiculo &taxi : listaTaxis)
    {
        if (taxi.placa == placa)
        {
            cout << "taxi con placas " << placa << " ya existe" << endl;
            return false;
        }
    }

    // validar año de vehiculo
    if (year < 2010)
    {
        cout << "El año del vehiculo permitido es 2010 o superior" << endl;
        return false;
    }

    return true; // si ambas condiciones se cumplen
}

void reingresarTaxi(vector<Vehiculo> &listaTaxis, const string &placaBuscada)
{
    Vehiculo taxi;
    bool taxiEncontrado = false;
    for (size_t i = 0; i < listaTaxis.size(); ++i) // size_t es como int pero solo con valores positivos
    {
        if (listaTaxis[i].placa == placaBuscada && listaTaxis[i].estado == "En ruta")
        {
            listaTaxis[i].estado = "Disponible"; // Cambiar el estado

            Vehiculo taxiReingresado = listaTaxis[i]; // Copiar
            listaTaxis.erase(listaTaxis.begin() + i); // Eliminar
            listaTaxis.push_back(taxiReingresado);    // Agregar al final
            guardarVehiculo(listaTaxis, "CARS_STORAGE.txt");
            limpiarPantalla();
            cout << "**********************************************************************************\n";
            cout << "Taxi con placa " << placaBuscada << " reingresado como Disponible y enviado al final de la cola.\n";
            taxiEncontrado = true;
            break;
        }
    }
    if (!taxiEncontrado)
    {
        limpiarPantalla();
        cout << "**************************************************************************************\n";
        cout << "El Taxi " << placaBuscada << " no coincide con Registros. o ya se encuentra en base y Disponible";
    }
}

void gananciasMesY(vector<Transaction> &viajes, string &yearMes)
{
    bool existeTransaccion = false;
    string fechaFormateada = "MES: " + yearMes.substr(6, 7) + " DEL AÑO " + yearMes.substr(0, 4); // Extrae Año y mes y reordena a mes - Año
    float total = 0.0;

    for (const auto &viaje : viajes)
    {
        string fecha = viaje.fechaViaje.substr(0, 7); // Extrae "YYYY-MM"
        if (fecha == yearMes)
        {
            total += viaje.tarifa;
            existeTransaccion = true;
        }
    }

    if (existeTransaccion)
    {
        limpiarPantalla();
        cout << "*******************************************************************\n";
        cout << "REPORTE DE INGRESOS PARA EL MES: " << fechaFormateada << "\n";
        cout << "*******************************************************************\n\n";
        cout << fixed << setprecision(2);
        cout << "Total de ingresos para el " << fechaFormateada << " :    $" << total << "\n";
    }
    else
    {
        limpiarPantalla();
        cout << "*************************************************************************************\n";
        cout << "NO SE ENCONTRARON REGISTROS PARA LA CONSULTA SOLICITADA: " << fechaFormateada << "* ---" << endl
             << endl;
    }
}

void gananciasMesYearPlaca(vector<Transaction> &viajes, string &yearMes, string &placa)
{
    bool existeTransaccion = false;
    string fechaFormateada = "MES: " + yearMes.substr(6, 7) + " DEL AÑO " + yearMes.substr(0, 4); // Extrae Año y mes y reordena a mes - Año
    float total = 0.0;

    for (const auto &viaje : viajes)
    {
        if (viaje.placa == placa)
        {
            string fecha = viaje.fechaViaje.substr(0, 7); // Extrae "YYYY-MM"
            if (fecha == yearMes)
            {
                total += viaje.tarifa;
                existeTransaccion = true;
            }
        }
    }

    if (existeTransaccion)
    {
        limpiarPantalla();
        cout << "*******************************************************************\n";
        cout << "REPORTE DE INGRESOS PARA EL MES: " << fechaFormateada << "\n";
        cout << "PLACA: " << placa << "\n";
        cout << "*******************************************************************\n\n";
        cout << fixed << setprecision(2);
        cout << "Total de ingresos para el vehiculo placas: " << placa << " :    $" << total << "\n";
    }
    else
    {
        limpiarPantalla();
        cout << "*************************************************************************************\n";
        cout << "NO SE ENCONTRARON REGISTROS PARA LA CONSULTA SOLICITADA. Placa: " << placa << "* ---" << endl
             << endl;
    }
}