#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

// Se define la estructura que sirve como almacenamiento para la orientación del sol
typedef struct {
    double azimut; // Azimut (ángulo en el plano horizontal desde el norte)
    double elevacion; // Elevación (ángulo desde el horizonte)
} SolPosicion;

// Función para calcular la posición del sol
SolPosicion calculoSolPosicion(double latitud, double longitud, struct tm *timeinfo);

// Función que nos ayuda a convertir grados a radianes
double gradosARadianes(double grados) {
    return grados * M_PI / 180.0;
}

// Función que nos ayuda a convertir radianes a grados
double radianesAGrados(double radianes) {
    return radianes * 180.0 / M_PI;
}

// Función para calcular la posición del sol basada en una fórmula simplificada
SolPosicion calculoSolPosicion(double latitud, double longitud, struct tm *timeinfo) {
    SolPosicion solPos;

    // Se Convierte la hora local en hora solar
    int dayOfYear = timeinfo->tm_yday + 1; // tm_yday cuenta desde 0
    double hora = timeinfo->tm_hour + timeinfo->tm_min / 60.0 + timeinfo->tm_sec / 3600.0;

    // Se calcula la declinación solar
    double declinacion = 23.44 * sin(gradosARadianes(360.0 / 365.0 * (dayOfYear - 81)));

    // Se calcula el ángulo horario
    double medioDiaSolar = 12.0 - (4.0 * (longitud - 15.0 * round(longitud / 15.0))) / 60.0;
    double anguloHora = 15.0 * (hora - medioDiaSolar);

    // Se calcula la elevación solar usandp la fórmula específica 
    solPos.elevacion = radianesAGrados(asin(sin(gradosARadianes(latitud)) * sin(gradosARadianes(declinacion)) +
                                              cos(gradosARadianes(latitud)) * cos(gradosARadianes(declinacion)) * cos(gradosARadianes(anguloHora))));

    // Se debe calcular el azimut solar usando la fórmula proporcionada
    double elevacionRad = gradosARadianes(solPos.elevacion);
    double declinacionRad = gradosARadianes(declinacion);
    double latitudRad = gradosARadianes(latitud);

    double azimutRad = acos((sin(declinacionRad) - sin(elevacionRad) * sin(latitudRad)) / (cos(elevacionRad) * cos(latitudRad)));
    double azimut = radianesAGrados(azimutRad);

    // Ajustar el azimut para que se mida desde el sur en sentido antihorario
    if (anguloHora > 0) {
        azimut = 360.0 - azimut;
    }

    solPos.azimut = azimut;

    return solPos;
}

// Función para leer un double de forma segura
double leerDouble(const char *prompt) {
    double valor;
    char buffer[100];
    while (true) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            char *endptr;
            valor = strtod(buffer, &endptr);
            if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
                break;
            }
        }
        printf("Entrada inválida. Por favor ingrese un número.\n");
    }
    return valor;
}

// Función principal
int main() {
    // Leer la latitud y longitud de forma segura
    double latitud = leerDouble("Ingrese la latitud: ");
    double longitud = leerDouble("Ingrese la longitud: ");

    // Obtener la hora actual
    time_t t = time(NULL);
    struct tm *timeinfo = localtime(&t);

    // Calcular la posición del sol
    SolPosicion solPos = calculoSolPosicion(latitud, longitud, timeinfo);

    // Mostrar los resultados
    printf("Azimut: %.2f grados\n", solPos.azimut);
    printf("Elevacion: %.2f grados\n", solPos.elevacion);

    return 0;
}
