# Avance P.I.S GRUPO 9
Este repositorio contiene el código en C de nustro proyecto P.I.S el cuál trata de calcularnos la orientación del sol. 
Tambien adjuntaremos el informe realizado por el grupo 
# Los integrantes del grupo son: 
- Ana Panamito 
- Julian Vega 
- Steven Narváez 
- Fernando Ortega 
- Leonardo Sánchez
# Informe 
En este enlace esta nuestro informe: https://github.com/Sebaslsst/AvancePIS.Md/blob/1d267c2fc23a10fa22e03b687157546368a95741/Evaluaci%C3%B3n_Proyecto_PIS.pdf
# Código en C
**Orientacion del sol**

En este link adjunto el código en C: https://github.com/Sebaslsst/AvancePIS.Md/blob/main/posici%C3%B3nSol.c

## Estructura de la posición del sol: ##
double azimut: Almacena el ángulo del azimut, que representa la dirección del sol en el plano horizontal medido desde el norte.
double elevacion: Almacena el ángulo de elevación, que representa la altura del sol sobre el horizonte.

## Funciones auxiliares: ##
gradosARadianes(double grados): Convierte un ángulo de grados a radianes.
radianesAGrados(double radianes): Convierte un ángulo de radianes a grados.


## Función main: ##
double latitud: La latitud del lugar en grados.
double longitud: La longitud del lugar en grados.
time_t t: Variable para almacenar el tiempo actual en formato de tiempo.
struct tm *timeinfo: Es una estructura que almacena la hora local desglosada en componentes (año, mes, día, hora, etc.).

## Función calculoSolPosicion: ##
int dayOfYear: Almacena el día del año (de 1 a 365/366), calculado a partir de timeinfo->tm_yday + 1.
double hora: La hora del día en formato decimal (incluye minutos y segundos).
double declinacion: La declinación solar, que es el ángulo entre los rayos del sol y el plano del ecuador terrestre.
double medioDiaSolar: El mediodía solar, ajustado según la longitud del lugar.
double anguloHora: El ángulo horario, que es la diferencia entre la hora actual y el mediodía solar en términos de grados.
double azimutRadians: El azimut en radianes, calculado usando la función atan2.
double azimut: El azimut en grados, convertido a partir de azimutRadianes.


## Detalle de las variables y su cálculo ##

dayOfYear o diaDelAño:
int dayOfYear = timeinfo->tm_yday + 1;

Calcula el día del año. tm_yday es el día del año desde 0 (1 de enero es 0), por eso se le suma 1.
hora:
double hora = timeinfo->tm_hour + timeinfo->tm_min / 60.0 + timeinfo->tm_sec / 3600.0;

Convierte la hora actual en una representación decimal. Por ejemplo, 14:30:00 se convierte en 14.5.
declinación:
double declinacion = 23.44 * sin(degreesToRadians(360.0 / 365.0 * (dayOfYear - 81)));

Calcula la declinación solar en función del día del año. La declinación varía entre ±23.44 grados durante el año.
medioDiaSolar:
double medioDiaSolar = 12.0 - (4.0 * (longitude - 15.0 * round(longitude / 15.0))) / 60.0;

Calcula el mediodía solar ajustado por la longitud del lugar. Cada grado de longitud este/oeste ajusta el tiempo solar en 4 minutos.

## anguloHora: ##
double anguloHora = 15.0 * (hora - medioDiaSolar);

Calcula el ángulo horario en grados, que es la diferencia entre la hora actual y el mediodía solar multiplicada por 15 grados por hora.

## elevacion:##
solPos.elevacion = radianesAGrados (asin(sin(gradosARadianes (latitud)) * sin(gradosARadianes (declinacion)) + cos(gradosARadianes (latitud)) * cos(gradosARadianes (declinacion)) * cos(gradosARadianes (anguloHora))));

Calcula la elevación solar usando la fórmula de ángulo de elevación. La función asin devuelve el ángulo en radianes, que se convierte a grados.

## azimutRadianes: ##
double azimutRadianes = atan2(sin(gradosARadianes (anguloHora)), cos(gradosARadianes (anguloHora)) * sin(gradosARadianes (latitud)) - tan(gradosARadianes (declinacion)) * cos(gradosARadianes (latitud)));
Calcula el azimut en radianes utilizando atan2, que tiene en cuenta el signo y la cuadrante correcta del ángulo horario.

## azimut: ##
solPos.azimut = radianesAGrados (azimutRadianes);

Convierte el azimut de radianes a grados.
if (solPos.azimut < 0) { solPos.azimut  += 360.0; }

Ajusta el azimut para que esté en el rango de 0 a 360 grados.
solPos.azimut = fmod((sunPos.azimuth + 180.0), 360.0);

Ajusta el azimut para que se mida desde el sur en sentido antihorario, ósea se mide de sentido contrario a las manecillas del reloj.

