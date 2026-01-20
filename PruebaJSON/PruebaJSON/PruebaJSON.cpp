// PruebaJSON.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>

#include <string>

#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

int main()
{
    string route = "C:/Users/pablo/Desktop/Uni/Master/TFM/BTGenerator/Content/TFM/JSONs/Ejemplo1.json";
    fstream f(route);
    json data = json::parse(f);


    for (auto& it : data["Blackboard"]) {
        for (auto& ot : it.items()) {
            string s1 = ot.key();
            string s2 = ot.value();
            cout << s1 << " " << s2 << " " << typeid(s1).name() << "\n";
        }
    }
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
