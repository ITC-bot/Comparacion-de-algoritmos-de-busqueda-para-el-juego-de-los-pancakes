/*
Hecho por Marcos Abaroa
fecha del ultimo cambio:28/03/23
NOTAS GENERALES:
--IDA* sera superado por A y viceversa, depende mucho del como este programado y la configuracion recibida de pancakes*
--Si el algoritmo de busqueda en profundidad no encuentra solucion, simplemente se le da mas profundidad en el main
*/

#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <queue>
#include <algorithm>
#include <functional>
#include <math.h>

float inf = std::numeric_limits<float>::infinity();
using namespace std;

// Función que voltea los primeros k pancakes de la pila
void voltear(string& pancakes, int k) {
    int i = 0, j = k - 1;
    while (i < j) {
        char temp = pancakes[i];
        pancakes[i] = pancakes[j];
        pancakes[j] = temp;
        i++;
        j--;
    }
}

// Función que verifica si está ordenada
bool esta_ordenada(string pancakes) {
    for (int i = 1; i < pancakes.size(); i++) {
        if (pancakes[i] < pancakes[i-1]) {
            return false;
        }
    }
    return true;
}

// Función que genera los sucesores de una configuración
vector<string> generar_sucesores(string pancakes) {
    vector<string> sucesores;
    for (int i = 1; i <= pancakes.size(); i++) {
        string sucesor = pancakes;
        voltear(sucesor, i);
        sucesores.push_back(sucesor);
    }
    return sucesores;
}

// Función que genera una cadena aleatoria de caracteres que representen los pancakes a ordenar
string generar_caracteres_aleatorios(int n) {
    string pancakes = "";
    unordered_set<char> chars_set;
    srand(time(NULL));

    while (chars_set.size() < n) {
        char c = 'a' + rand() % 26;
        if (chars_set.find(c) == chars_set.end()) {
            chars_set.insert(c);
            pancakes += c;
        }
    }

    return pancakes;
}

// Función que realiza la búsqueda en amplitud
void bfs(string pancakes) {
    queue<string> cola;
    unordered_set<string> visitados;
    int count = 0;
    cola.push(pancakes);
    visitados.insert(pancakes);
    while (!cola.empty()) {
        string curr_pancakes = cola.front();
        cola.pop();
        count++;
        if (esta_ordenada(curr_pancakes)) {
            cout << "Solucion encontrada: " << curr_pancakes << endl;
            cout << "Numero de nodos visitados: " << count << endl;
            return;
        }
        vector<string> sucesores = generar_sucesores(curr_pancakes);
        for (string sucesor : sucesores) {
            if (visitados.find(sucesor) == visitados.end()) {
                cola.push(sucesor);
                visitados.insert(sucesor);
                count++;
            }
        }
    }
    cout << "No se encontro solucion." << endl;
    cout << "Numero de nodos visitados: " << count << endl;
}

// Función que realiza la búsqueda en profundidad
bool dfs(string curr, string end, unordered_set<string>& visited, int depth, int max_depth, int& count){
    visited.insert(curr);
    count++;
    if (curr == end){
        return true;
    }
    if (depth == max_depth){
        return false;
    }
    vector<string> sucesores = generar_sucesores(curr);
    for (string sucesor : sucesores){
        if (visited.count(sucesor) == 0){
            if (dfs(sucesor, end, visited, depth+1, max_depth, count)){
                return true;
            }
        }
    }
    return false;
}

// Función recursiva que realiza la búsqueda en profundidad iterativa
bool idfs(string pancakes, int depth, int& count) {
    count++;
    if (esta_ordenada(pancakes)) {
        cout << "Solucion encontrada: " << pancakes << endl;
        cout << "Numero de nodos visitados: " << count << endl;
        return true;
    }
    if (depth > pancakes.size()) {
        return false;
    }
    vector<string> sucesores = generar_sucesores(pancakes);
    for (string sucesor : sucesores) {
        bool encontrado = idfs(sucesor, depth + 1, count);
        if (encontrado) {
            return true;
        }
    }
    return false;
}

// Función que implementa la heuristica
int h4(const string& pancakes, const string& target) {
    int c = 0;
    for (int i = 0; i < pancakes.size(); i++) {
        if (pancakes[i] != target[i])
            c++;
        if (i < pancakes.size()-1 && abs(pancakes[i]-pancakes[i+1]) > 1)
            c++;
    }
    return c;
}

// Función que realiza la búsqueda A*
void a_star(string pancakes) {
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> cola;
    unordered_set<string> visitados;
    int count = 0;
    string target = pancakes;
    sort(target.begin(), target.end());
    cola.push({h4(pancakes, target), pancakes});
    visitados.insert(pancakes);
    while (!cola.empty()) {
        string curr_pancakes = cola.top().second;
        cola.pop();
        count++;
        if (curr_pancakes == target) {
            cout << "Solucion encontrada: " << curr_pancakes << endl;
            cout << "Numero de nodos visitados: " << count << endl;
            return;
        }
        vector<string> sucesores = generar_sucesores(curr_pancakes);
        for (string sucesor : sucesores) {
            if (visitados.find(sucesor) == visitados.end()) {
                cola.push({h4(sucesor, target), sucesor});
                visitados.insert(sucesor);
                count++;
            }
        }
    }
    cout << "No se encontro solucion." << endl;
    cout << "Numero de nodos visitados: " << count << endl;
}

// Función que implementa la búsqueda con IDA*
string ida_estrella(const string& pancakes, const string& target, float cost, float threshold, string path, int& nodos_visitados) {
    float f = cost + h4(pancakes, target);
    if (f > threshold) {
        return "inf";
    }
    if (esta_ordenada(pancakes)) {
        return pancakes;
    }
    string min_path = "inf";
    for (auto sucesor : generar_sucesores(pancakes)) {
        if (path.find(sucesor) != std::string::npos || cost + h4(sucesor, target) > threshold) {
            nodos_visitados++;
            continue;
        }
        float new_cost = cost + 1;
        string t = ida_estrella(sucesor, target, new_cost, threshold, path + sucesor + " ", nodos_visitados);
        if (t != "inf" && (min_path == "inf" || t.size() < min_path.size())) {
            min_path = t;
        }
    }
    return min_path;
}

//Funcion principal
int main() {

    int n;
    cout << "Comparacion de algoritmos de busqueda\n";
    cout << "Ingrese el numero de caracteres de pancakes: ";
    cin >> n;
    string pancakes = generar_caracteres_aleatorios(n);
    cout << "Pila de pancakes original: " << pancakes << endl;
    cout <<" "<<endl;
    //BUSQUEDA EN AMPLITUD
    cout << "--Busqueda en amplitud--";
    cout <<" "<<endl;
    bfs(pancakes);
    cout <<" "<<endl;
    //BUSQUEDA EN PROFUNDIDAD
    cout << "--Busqueda en profundidad--";
    cout <<" "<<endl;
    string pancakeEnd = pancakes;
    sort(pancakeEnd.begin(), pancakeEnd.end());
    unordered_set<string> visited;
    int count = 0;
    int max_depth = 100; // profundidad máxima de la búsqueda
    bool found = dfs(pancakes, pancakeEnd, visited, 0, max_depth, count);
    cout << "Solucion encontrada: " << pancakeEnd << endl;
    if (found){
        cout << "Numero de nodos visitados: " << count << endl;
    }
    else {
        cout << "No se encontro una solucion" << endl;
    }
    cout <<" "<<endl;
    //BUSQUEDA EN PROFUNDIDAD ITERATIVA
    cout << "--Busqueda en profundidad iterativa--"<<endl;;
    int count2 = 0;
    bool encontrado = false;
    for (int depth = 1; depth <= pancakes.size(); depth++) {
        encontrado = idfs(pancakes, depth, count2);
        if (encontrado) {
            break;
        }
    }
    if (!encontrado) {
        cout << "No se encontro solucion." << endl;
        cout << "Numero de nodos visitados: " << count2 << endl;
    }
    cout <<" "<<endl;
    //BUSQUEDA A*
    cout << "--Busqueda A*--"<<endl;;
    a_star(pancakes);
    //BUSQUEDA IDA*
    cout <<" "<<endl;
    cout << "--Busqueda A*--"<<endl;;
    string target = pancakes;
    sort(target.begin(), target.end());  // Pila objetivo ordenada
    float threshold = h4(pancakes, target);  // Umbral inicial
    int nodos_visitados = 0;
    while (true) {
        string ruta_movimiento = ida_estrella(pancakes, target, 0, threshold, "", nodos_visitados);
        if (ruta_movimiento == "inf") {
            // Si la ruta de movimiento mínima no fue encontrada, aumenta el umbral
            threshold++;
        } else {
            // Si se encontró la ruta de movimiento mínima, finaliza el ciclo
            cout << "Solucion encontrada : " << ruta_movimiento << endl;
            break;
        }
    }
    cout << "Numero de nodos visitados: " << nodos_visitados << endl;

    return 0;
}

