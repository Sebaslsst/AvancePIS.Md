#include <stdio.h>
#include <math.h>
#include <time.h>

// Definimos la estructura que sirve como almacenamiento para la orientación del sol
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

// Función que nos ayuda convertir radianes a grados
double radianesAGrados(double radianes) {
    return radianes * 180.0 / M_PI;
}

// Función para calcular la posición del sol basada en una fórmula simplificada
SolPosicion calculoSolPosicion(double latitud, double longitud, struct tm *timeinfo) {
    SolPosicion solPos;

    // Convertir la hora local en hora solar
    // dayOfYear es los dias contados desde el 1 de enero 
    int dayOfYear = timeinfo->tm_yday + 1; // tm_yday cuenta desde 0
    double hora = timeinfo->tm_hour + timeinfo->tm_min / 60.0 + timeinfo->tm_sec / 3600.0;

    // Calcular la declinación solar
    double declinacion = 23.44 * sin(gradosARadianes(360.0 / 365.0 * (dayOfYear - 81)));

    // Calcular el ángulo horario
    double medioDiaSolar = 12.0 - (4.0 * (longitud - 15.0 * round(longitud / 15.0))) / 60.0;
    double anguloHora = 15.0 * (hora - medioDiaSolar);

    // Calcular la elevación solar
    solPos.elevacion = radianesAGrados(asin(sin(gradosARadianes(latitud)) * sin(gradosARadianes(declinacion)) +
                                              cos(gradosARadianes(latitud)) * cos(gradosARadianes(declinacion)) * cos(gradosARadianes(anguloHora))));

    // Calcular el azimut solar
    solPos.azimut = radianesAGrados(atan2(-sin(gradosARadianes(anguloHora)),
                                            cos(gradosARadianes(anguloHora)) * sin(gradosARadianes(latitud)) -
                                            tan(gradosARadianes(declinacion)) * cos(gradosARadianes(latitud))));

    if (solPos.azimut < 0) {
        solPos.azimut += 360.0;
    }
    
    return solPos;
}

// Función principal, usamos dos variables la latitud y longitud  
int main() {
    double latitud, longitud;
    time_t t = time(NULL);
    struct tm *timeinfo = localtime(&t);

    // Se pide al usuario ingresar la latitud y longitud
    printf("Ingrese la latitud: ");
    scanf("%lf", &latitud);
    printf("Ingrese la longitud: ");
    scanf("%lf", &longitud);

    // Calcular la posición del sol
    SolPosicion solPos = calculoSolPosicion(latitud, longitud, timeinfo);

    // Mostrar los resultados
    printf("Azimut: %.2f grados\n", solPos.azimut);
    printf("Elevacion: %.2f grados\n", solPos.elevacion);

    return 0;
}
