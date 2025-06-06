#include <iostream>
#include <string>
#include <iomanip>  //header para convertir float a 2 cifras
#include <limits>
#include <fstream>
#include <filesystem>
#include <vector>
#include <ctime>         //libreria para manejar hora
#include <sstream>       //libreria para manejar hora
#include <unordered_set> // almacena elementos únicos sin orden específico,
#include "Structs_utils.h"

using namespace std;

//**************************************************************************************************************************//
//--------------------------------------------------------------------------------------------------------------------------//
//**************************************************************************************************************************//
int main()
{
    // Variables a Utilizar: Opcion
    char menu;
    char menu2;
    char continuar;
    char catSelecionada;
    vector<Vehiculo> listaTaxis;
    vector<Transaction> transacciones;
    Vehiculo taxi;
    Transaction viaje;
    string vehicleLabel[4] = {"1.Placa:", "2.Marca:", "3.Modelo:", "4.Año:"};
    string conductorLabel[4] = {"1.Documento de Identidad:", "2.Nombre Completo:", "3.Telefono/Cel:", "4.Dirección:"};
    string transacLabel[4] = {"Origen del viaje:", "Destino del viaje:", "Tarifa establecida:", "Fecha del viaje:"};
    TextTable tabla('-', '|', '+'); // creacion de tabla con bordes y vacia
    // variables temporales
    string placaBuscada;         // Placa para reingresar taxi a cola y para buscar en reportes
    string temp;                 // convertir año de vehiculo a tipo numerico
    string temp2;                // convertir tarifa a float
    string tempDia;              // obtener fecha de viaje
    string tempMes;              // obtener fecha de viaje
    string tempYear;             // obtener fecha de viaje
    bool taxiEncontrado = false; // para buscar taxi
    cargarFlotaTaxis(listaTaxis, "CARS_STORAGE.txt");
    cargarTransacciones(transacciones, "TRANSACTION_LOG");

    limpiarPantalla();
    do
    {
        cout << "BIENVENIDO AL SISTEMA DE GESTION DE TAXIS 'TRUE-DRIVER'!" << endl;
        cout << "------------------------------------------------------------------" << endl;
        cout << " Ingrese la opción de la operacion que desea realizar:" << endl;
        cout << " Seleccione Una opcion Del menu que se presenta a continuacion:" << endl
             << endl;
        cout << "\t a. -- Registrar nuevo Taxi y Conductor" << endl;
        cout << "\t b. -- Enviar Taxi a cliente" << endl;
        cout << "\t c. -- Reingresar Taxi a la cola de Espera" << endl;
        cout << "\t d. -- Consulta de vehiculos en Ruta" << endl;
        cout << "\t e. -- Consulta de vehiculos disponibles" << endl;
        cout << "------------------------------------------------------------------\n"
             << endl;
        cout << "\t f. -- VER REPORTES" << endl;
        cout << "\t z. -- Salir\n"
             << endl;
        cout << "Ingrese la opcion seleccionada: ";
        cin >> menu;

        switch (menu)
        {
        case 'a': // REGISTRO DE TAXIS NUEVOS + CONDUCTORES
            limpiarPantalla();
            cout << "**********************************************************************\n";
            cout << "Ingreso de nuevo Vehiculo.\n";

            // imprime campos/labels de Formulario//
            for (int i = 0; i < 4; i++)
            {
                gotoxy(3, 5 + i * 2);
                cout << vehicleLabel[i];
                gotoxy(18, 5 + i * 2);
                cout << "_______________________________";
            };
            cout << "\n \n";

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar el buffer

            gotoxy(19, 5);
            getline(cin, taxi.placa);
            gotoxy(19, 7);
            getline(cin, taxi.marca);
            gotoxy(19, 9);
            getline(cin, taxi.modelo);
            gotoxy(19, 11);
            getline(cin, temp);     // captura variable temporal para año
            taxi.year = stoi(temp); // asigna variable real a la estructura
            cout << "\n \n";

            // asignar categoria
            if (taxi.year >= 2015)
            {
                taxi.categoria = "Ejecutiva";
            }
            else
            {
                taxi.categoria = "Tradicional";
            }
            // asignar estado como disponible
            taxi.estado = "Disponible";

            if (validarTaxiNuevo(listaTaxis, taxi.placa, taxi.year)) // Validar año y placa del taxi
            {
                cout << "El registro de Taxi cumple con los requerimientos" << endl;
            }
            else
            {
                cout << "El Registro no se ha podido guardar." << endl;
                break;
            }

            // INGRESO DE CONDUCTOR ASOCIADO
            limpiarPantalla();
            cout << "**********************************************************************\n";
            cout << "\n<<<Registro de Conductor Asociado a taxi con Placas>>> " << "<<" << taxi.placa << ">>\n";
            cout << "---------------------------------------------------------------------- \n";
            cout << "\n";
            // imprime campos/labels de Formulario//
            for (int i = 0; i < 4; i++)
            {
                gotoxy(3, 8 + i * 2);
                cout << conductorLabel[i];
                gotoxy(30, 8 + i * 2);
                cout << "_________________________________________________";
            };

            gotoxy(31, 8);
            getline(cin, taxi.conductor.dui);
            gotoxy(31, 10);
            getline(cin, taxi.conductor.nombre);
            gotoxy(31, 12);
            getline(cin, taxi.conductor.telefono);
            gotoxy(31, 14);
            getline(cin, taxi.conductor.direccion);
            cout << "\n \n";

            listaTaxis.push_back(taxi);                      // subimos el taxi guardado al vector
            guardarVehiculo(listaTaxis, "CARS_STORAGE.txt"); // agregamos el vector actualizado al archivo txt

            limpiarPantalla();
            cout << "**********************************************************************\n";
            cout << "Nuevo Taxi registrado nexitosamente." << endl;
            cout << "Placas:        " << taxi.placa << endl;
            cout << "Conductor:     " << taxi.conductor.nombre << endl;

            break;

        case 'b': // REGISTRO DE VIAJES

            limpiarPantalla();
            cout << "Seleccione la categoría del Taxi Solicitado por cliente:" << endl;

            cout << "\n \t • a. \t Ejecutiva";
            cout << "\n \t • b. \t Tradicional" << endl;
            cout << "\nIngrese la opcion seleccionada: ";
            cin >> catSelecionada;

            cout << "---------------------------------------------------------" << endl;
            cout << "\n";
            switch (catSelecionada)
            {
            case 'a': // Ejecutiva
            {
                // Seleccionar Taxi
                taxiEncontrado = false;
                for (auto &taxi : listaTaxis)
                {
                    if (taxi.categoria == "Ejecutiva" && taxi.estado == "Disponible")
                    {
                        limpiarPantalla();
                        cout << "**********************************************************************\n";
                        cout << "\n<<<Asignacion de viaje para vehiculo con placas>>> " << "<<" << taxi.placa << ">>\n";
                        cout << "\n<<<Conductor:>>> " << "<<" << taxi.conductor.nombre << ">>\n";
                        cout << "\n<<<Linea:>>> " << "<<" << taxi.categoria << ">>\n";
                        cout << "---------------------------------------------------------------------- \n";
                        cout << "\n";
                        // imprime campos/labels de Formulario//
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar el buffer
                        for (int i = 0; i < 3; i++)
                        {
                            gotoxy(3, 10 + i * 2);
                            cout << transacLabel[i];
                            gotoxy(25, 10 + i * 2);
                            cout << "_________________________________________";
                        };

                        gotoxy(3, 16);
                        cout << "Fecha de Viaje:       ___ -  ___ - _____    |    Formato Requerido: 00 00 0000" << endl;
                        cout << "                        dia /  mes /  año";

                        // captura de datos y construccion de estructura
                        gotoxy(26, 10);
                        getline(cin, viaje.origenDestino);
                        gotoxy(26, 12);
                        getline(cin, temp2);
                        viaje.origenDestino = viaje.origenDestino + " - " + temp2;
                        gotoxy(26, 14);
                        getline(cin, temp2);        // captura variable temporal para tarifa
                        viaje.tarifa = stof(temp2); // asigna variable real a la estructura
                        gotoxy(26, 16);
                        getline(cin, tempDia); // captura el dia del viaje
                        gotoxy(33, 16);
                        getline(cin, tempMes); // captura el mes del viaje
                        gotoxy(39, 16);
                        getline(cin, tempYear); // captura el año del viaje
                        viaje.fechaViaje = tempYear + "-" + tempMes + "-" + tempDia;
                        viaje.placa = taxi.placa;
                        viaje.conductor = taxi.conductor.dui;
                        viaje.hora = obtenerHoraActual();
                        transacciones.push_back(viaje); // carga info al vector de transacciones
                        taxi.estado = "En ruta";        // cambia el estado del taxi en el vector de lista de taxis
                        taxiEncontrado = true;
                        temp2 = taxi.conductor.nombre;
                        break; // sale del if y del bucle
                    }
                }
                if (!taxiEncontrado)
                {
                    cout << "No hay Taxis Ejecutivos disponibles por el momento";
                    break;
                }

                guardarViaje(transacciones, "TRANSACTION_LOG");  // guarda log de viaje
                guardarVehiculo(listaTaxis, "CARS_STORAGE.txt"); // actualiza archivo de la flota

                // Bucle que imprime cada transacción //SOLO PARA CONTROL DE DATOS PROCESADOS

                limpiarPantalla();
                cout << "**********************************************************************\n";
                cout << "\n--- Datos del Viaje Registrado---" << endl;
                cout << "Placa: " << viaje.placa << endl;
                cout << "Conductor: " << temp2 << endl;
                cout << "Ruta: " << viaje.origenDestino << endl;
                cout << "Tarifa: $ / " << viaje.tarifa << endl;
                cout << "Fecha: " << viaje.fechaViaje << endl;
                cout << "Hora: " << viaje.hora << endl;

                break;
            }

            case 'b': // Tradicional
            {
                // Seleccionar Taxi
                bool taxiEncontrado = false;
                for (auto &taxi : listaTaxis)
                {
                    if (taxi.categoria == "Tradicional" && taxi.estado == "Disponible")
                    {
                        limpiarPantalla();
                        cout << "**********************************************************************\n";
                        cout << "\n<<<Asignacion de viaje para vehiculo con placas>>> " << "<<" << taxi.placa << ">>\n";
                        cout << "\n<<<Conductor:>>> " << "<<" << taxi.conductor.nombre << ">>\n";
                        cout << "\n<<<Linea:>>> " << "<<" << taxi.categoria << ">>\n";
                        cout << "---------------------------------------------------------------------- \n";
                        cout << "\n";
                        // imprime campos/labels de Formulario//
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar el buffer
                        for (int i = 0; i < 3; i++)
                        {
                            gotoxy(3, 10 + i * 2);
                            cout << transacLabel[i];
                            gotoxy(25, 10 + i * 2);
                            cout << "_________________________________________";
                        };

                        gotoxy(3, 16);
                        cout << "Fecha de Viaje:       ___ -  ___ - _____    |    Formato Requerido: 00 00 0000" << endl;
                        cout << "                        dia /  mes /  año";

                        // captura de datos y construccion de estructura
                        gotoxy(26, 10);
                        getline(cin, viaje.origenDestino);
                        gotoxy(26, 12);
                        getline(cin, temp2);
                        viaje.origenDestino = viaje.origenDestino + " - " + temp2;
                        gotoxy(26, 14);
                        getline(cin, temp2);        // captura variable temporal para tarifa
                        viaje.tarifa = stof(temp2); // asigna variable real a la estructura
                        gotoxy(26, 16);
                        getline(cin, tempDia); // captura el dia del viaje
                        gotoxy(33, 16);
                        getline(cin, tempMes); // captura el mes del viaje
                        gotoxy(39, 16);
                        getline(cin, tempYear); // captura el año del viaje
                        viaje.fechaViaje = tempYear + "-" + tempMes + "-" + tempDia;
                        viaje.placa = taxi.placa;
                        viaje.conductor = taxi.conductor.dui;
                        viaje.hora = obtenerHoraActual();
                        transacciones.push_back(viaje); // carga info al vector de transacciones
                        taxi.estado = "En ruta";        // cambia el estado del taxi en el vector de lista de taxis
                        taxiEncontrado = true;
                        temp2 = taxi.conductor.nombre;
                        break; // sale del if y del bucle
                    }
                }
                if (!taxiEncontrado)
                {
                    cout << "No hay Taxis de linea Tradicional disponibles por el momento";
                    break;
                }

                guardarViaje(transacciones, "TRANSACTION_LOG");  // guarda log de viaje
                guardarVehiculo(listaTaxis, "CARS_STORAGE.txt"); // actualiza archivo de la flota

                // Bucle que imprime cada transacción //SOLO PARA CONTROL DE DATOS PROCESADOS

                limpiarPantalla();
                cout << "**********************************************************************\n";
                cout << "\n--- Datos del Viaje Registrado---" << endl;
                cout << "Placa: " << viaje.placa << endl;
                cout << "Conductor: " << temp2 << endl;
                cout << "Ruta: " << viaje.origenDestino << endl;
                cout << "Tarifa: $ / " << viaje.tarifa << endl;
                cout << "Fecha: " << viaje.fechaViaje << endl;
                cout << "Hora: " << viaje.hora << endl;

                break;
            }
            }
            break;

        case 'c': // REINGRESO DE TAXI A BASE
            limpiarPantalla();
            cout << "*************************************************************************************\n";
            cout << "\n--- Ingrese la Placa del vehiculo que desea Reingresar a la Cola de espera ---" << endl;
            cout << "\n--- (sin espacios ni guiones) ---" << endl
                 << endl;
            cin >> placaBuscada;
            reingresarTaxi(listaTaxis, placaBuscada);
            break;

        case 'd': // CONSULTA VEHICULOS EN RUTA
            limpiarPantalla();
            cout << "*************************************************************************************\n";
            cout << "\n--- *Vehiculos En Ruta* ---" << endl
                 << endl;
            tabla = TextTable('-', '|', '+'); // resetea la tabla
            tabla.add("Placa");
            // tabla.add("Marca");
            tabla.add("Modelo");
            tabla.add("Year");
            tabla.add("  Conductor");
            tabla.add("Categoria");
            tabla.add("  En ruta");
            tabla.endOfRow();

            for (auto &taxi : listaTaxis)
            {
                if (taxi.estado == "En ruta")
                {
                    for (auto &viaje : transacciones)
                    {
                        if (taxi.placa == viaje.placa)
                        {
                            temp2 = viaje.origenDestino;
                        }
                    }
                    tabla.add(taxi.placa);
                    // tabla.add(taxi.marca);
                    tabla.add(taxi.modelo);
                    tabla.add(to_string(taxi.year));
                    tabla.add(taxi.conductor.nombre);
                    tabla.add(taxi.categoria);
                    tabla.add(temp2);
                    tabla.endOfRow();
                }
            }
            cout << tabla;
            break;

        case 'e': // CONSULTA VEHICULOS DISPONIBLES
            limpiarPantalla();
            cout << "*************************************************************************************\n";
            cout << "\n--- *Vehiculos Disponibles para viaje* ---" << endl
                 << endl;
            tabla = TextTable('-', '|', '+'); // resetea la tabla
            tabla.add("Placa");
            tabla.add("Marca");
            tabla.add("Modelo");
            tabla.add("Year");
            tabla.add("  Conductor");
            tabla.add("Categoria");
            tabla.endOfRow();

            for (auto &taxi : listaTaxis)
            {
                if (taxi.estado == "Disponible")
                {
                    tabla.add(taxi.placa);
                    tabla.add(taxi.marca);
                    tabla.add(taxi.modelo);
                    tabla.add(to_string(taxi.year));
                    tabla.add(taxi.conductor.nombre);
                    tabla.add(taxi.categoria);
                    tabla.endOfRow();
                }
            }
            cout << tabla;
            break;

        case 'f': // REPORTERÍA
        {
            limpiarPantalla();
            bool salirReportes = false;
            string tarifaString;
            unordered_set<string> placas_con_viajes; // Almacena placas con al menos 1 viaje(contenedor aux con tabla hash)
            do
            {
                cout << "*********************************Reportes***********************************" << endl;
                cout << "-----------------------------------------------------------------------------" << endl
                     << endl;
                cout << "Seleccione Una opcion Del menu que se presenta a continuacion:" << endl
                     << endl;
                cout << "a. -- Listado de viajes realizados por cada vehículo" << endl;
                cout << "b. -- Listado de viajes realizados por un vehículo específico" << endl;
                cout << "c. -- Ingresos totales obtenidos en un mes" << endl;
                cout << "d. -- Ingresos totales obtenidos en un mes por un vehículo específico" << endl;
                cout << "x. -- Regresar a menú Principal\n"
                     << endl;
                cout << "Ingrese la opcion seleccionada: ";
                cin >> menu2;

                switch (menu2)
                {
                case 'a': // viajes realizados por cada vehículo (Todos los vehículos)
                    limpiarPantalla();
                    cout << "*************************************************************************************\n";
                    cout << "\n--- *Viajes realizados por cada vehículo* ---" << endl
                         << endl;

                    // recorrer transacciones
                    for (const auto &viaje : transacciones)
                    {
                        placas_con_viajes.insert(viaje.placa); // inserta en tabla hash de apoyo la placa encontrada solo UNA VEZ(descarta repetidos)
                    }

                    // Mostrar solo taxis con viajes
                    for (const auto &taxi : listaTaxis)
                    {
                        // Si el taxi NO tiene viajes, lo saltamos
                        if (placas_con_viajes.find(taxi.placa) == placas_con_viajes.end())
                        {
                            continue;
                        }

                        // Mostrar info del taxi (solo si tiene viajes)
                        cout << "Viajes realizados por Taxi con placas: " << taxi.placa << endl;
                        cout << "Conductor: " << taxi.conductor.nombre << endl;
                        cout << "Categoría: " << taxi.categoria << endl;

                        // Crear tabla para los viajes de ESTE taxi
                        TextTable tabla('-', '|', '+');
                        tabla.add("       Ruta");
                        tabla.add("Tarifa");
                        tabla.add("Fecha");
                        tabla.endOfRow();

                        // Filtrar y mostrar solo los viajes de este taxi
                        for (const auto &viaje : transacciones)
                        {
                            if (viaje.placa == taxi.placa)
                            {
                                tabla.add(viaje.origenDestino);
                                tarifaString = to_string(viaje.tarifa);                            // convierte float en string
                                tarifaString = tarifaString.substr(0, tarifaString.find('.') + 3); // convierte float en string
                                tabla.add(tarifaString);
                                tabla.add(viaje.fechaViaje);
                                tabla.endOfRow();
                            }
                        }

                        cout << tabla << endl
                             << endl;
                    }

                    break;
                case 'b':                             // viajes realizados por un vehículo específico
                    tabla = TextTable('-', '|', '+'); // resetea la tabla
                    temp2 = "";                       // reset variable a utilizarse
                    taxiEncontrado = false;           // reset variable a utilizarse
                    limpiarPantalla();
                    cout << "*************************************************************************************\n";
                    cout << "\n--- Ingrese la Placa del vehiculo que desea Buscar para generar Reporte ---" << endl;
                    cout << "\n--- (sin espacios ni guiones) ---" << endl
                         << endl;
                    cin >> placaBuscada;
                    for (auto &taxi : listaTaxis)
                    {
                        if (taxi.placa == placaBuscada)
                        {
                            for (auto &viaje : transacciones)
                            {
                                if (viaje.placa == placaBuscada)
                                {
                                    temp2 = taxi.conductor.nombre;
                                    taxiEncontrado = true;
                                }
                            }
                        }
                    }

                    if (taxiEncontrado)
                    {
                        limpiarPantalla();
                        cout << "*************************************************************************************\n";
                        cout << "\n--- *Viajes realizados por vehiculo con placas: " << placaBuscada << "* ---" << endl
                             << endl;
                        tabla.add("       Ruta");
                        tabla.add("Tarifa");
                        tabla.add("Fecha");
                        tabla.endOfRow();

                        // inicia impresion de Datos
                        cout << "--- *Conductor: " << temp2 << "* ---" << endl
                             << endl;
                        for (auto &viaje : transacciones)
                        {
                            if (viaje.placa == placaBuscada)
                            {

                                tabla.add(viaje.origenDestino);
                                tarifaString = to_string(viaje.tarifa);                            // convierte float en string
                                tarifaString = tarifaString.substr(0, tarifaString.find('.') + 3); // convierte float en string
                                tabla.add(tarifaString);
                                tabla.add(viaje.fechaViaje);
                                tabla.endOfRow();
                            }
                        }

                        cout << tabla << endl
                             << endl;
                    }
                    else
                    {
                        limpiarPantalla();
                        cout << "*************************************************************************************\n";
                        cout << "NO SE ENCONTRARON REGISTROS PARA LA CONSULTA SOLICITADA. Placa: " << placaBuscada << "* ---" << endl
                             << endl;
                    }

                    break;
                case 'c':          // Ingresos totales obtenidos en un mes (especificado al generar el reporte).
                    tempYear = ""; // reset variables a utilizar
                    tempMes = "";
                    temp2 = "";
                    limpiarPantalla();
                    cout << "********************Ingresos totales obtenidos en un mes********************\n";
                    cout << "\n--- Ingrese el mes y el año que desea Buscar para generar Reporte ---" << endl;
                    cout << "\n \nMes: ___      Año: _____       |       Formato Requerido: 00 0000" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar el buffer
                    gotoxy(7, 6);
                    cin >> tempMes;
                    gotoxy(21, 6);
                    cin >> tempYear;
                    temp2 = tempYear + "-" + tempMes; // formamos el mesYear para la consulta

                    gananciasMesY(transacciones, temp2); // envia datos a la funcion y ella se encarga de todo.

                    break;
                case 'd':          // Ingresos totales obtenidos en un mes por un vehículo específico
                    tempYear = ""; // reset variables a utilizar
                    tempMes = "";
                    temp2 = "";
                    placaBuscada = "";
                    limpiarPantalla();
                    cout << "****************Ingresos totales obtenidos en un mes por vehiculo****************\n";
                    cout << "\n--- Ingrese el mes año y placa que desea Buscar para generar Reporte ---" << endl;
                    cout << "\n \nMes: ___      Año: _____       |       Formato Requerido: 00 0000" << endl;
                    cout << "\n \nPlaca: _________________       |       Formato Requerido: Sin guiones ni espacios" << endl;

                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar el buffer
                    gotoxy(7, 6);
                    cin >> tempMes;
                    gotoxy(21, 6);
                    cin >> tempYear;
                    temp2 = tempYear + "-" + tempMes; // formamos el mesYear para la consulta
                    gotoxy(10, 9);
                    cin >> placaBuscada;

                    gananciasMesYearPlaca(transacciones, temp2, placaBuscada); // envia datos a la funcion y ella se encarga de todo.

                    break;
                case 'x':
                    limpiarPantalla();
                    salirReportes = true;
                    continuar = 's'; // Esto evita que pregunte nuevamente al salir
                    break;
                default:
                    cout << "Opción no válida. Presione Enter para continuar...";
                    cin.ignore();
                    cin.get();
                    limpiarPantalla();
                    break;
                }

                if (!salirReportes && menu2 >= 'a' && menu2 <= 'd')
                {
                    cout << "\n\n¿Desea realizar otra operación en reportes? (s/n): ";
                    char continuarReportes;
                    cin >> continuarReportes;
                    if (continuarReportes == 'n' || continuarReportes == 'N')
                    {
                        salirReportes = true;
                    }
                    limpiarPantalla();
                }
            } while (!salirReportes);

            // Al salir del bucle de reportes, continuará con el bucle principal
            continue; // Esto hace que salte directamente a la siguiente iteración del bucle principal
        }
        break;

        case 'z': // SALIR
            limpiarPantalla();
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << "Gracias por utilizar el sistema..." << endl;
            exit(0);
            break;
        }
        cout << "\n \n¿Desea realizar otra operacion en el sistema? Presione Cualquier Tecla, Salir presione n o N." << endl;
        cin >> continuar;
        limpiarPantalla();

    } while (continuar != 'n' && continuar != 'N');
    cout << "Gracias por utilizar el sistema..." << endl
         << endl;

    return 0;
}