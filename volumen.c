/*
Volumen de un sólido de revolución por el método de Monte Carlo
Partimos de la función de la integral de Gauss, 𝑓(𝑥) = 𝑒−𝑥2, que cumple −∞∞∫ 𝑒−𝑥2 𝑑𝑥 = √𝜋.  Al girar su gráfica 360° alrededor de 
un eje se obtiene un sólido de revolución. El objetivo es calcular su volumen en C por el método de Monte Carlo, generando puntos 
aleatorios (x, y, z) en el espacio, en dos casos: girando la curva alrededor del eje x y alrededor del eje y.

Figura 1. En naranja, la curva y = e^(−x²); en azul, el sólido que genera al girar. La caja discontinua es la región donde se
generan los puntos aleatorios.

(a) Giro alrededor del eje x. Cada corte perpendicular al eje x es un círculo de radio 𝑟 = 𝑒−𝑥2. Un punto (x, y, z) está dentro 
del sólido si su distancia al eje x es menor que ese radio:
𝑦² + 𝑧² ≤ 𝑒(−2𝑥²)

(b) Giro alrededor del eje y. El sólido es una campana de altura 1 cuya superficie es 𝑦 = 𝑒−(𝑥²+𝑧²). Un punto (x, y, z), con y ≥ 0, 
está dentro si queda por debajo de la superficie:
𝑦 ≤ 𝑒−(𝑥²+𝑧²)


Resultado analítico
(a) Eje x. Sumando discos de radio 𝑒−𝑥² y usando∫ 𝑒−𝑎𝑥2 𝑑𝑥 = √𝜋/𝑎:
𝑉 = 𝜋 −∞∞∫ 𝑒−2𝑥²
𝑑𝑥 = 𝜋 · √(𝜋/2) ≈ 3,9374

(b) Eje y. Sumando capas cilíndricas de radio x y altura 𝑒−𝑥2:
𝑉 = ∫ 0∞∫2𝜋𝑥𝑒(−𝑥²)𝑑𝑥 = 𝜋 ≈ 3,1416

En el caso (b) se integra solo desde 0, porque las mitades x < 0 y x > 0 de la curva barren el mismo sólido al girar alrededor 
del eje y.

Pasos para programarlo en C:

1. Caja de muestreo. La función no se anula nunca, así que hay que cortar el dominio en |x| ≤ L. Toma L = 4, para este valor la 
función es práticamente nula (𝑒−16 ≈ 10⁻⁷). Para el caso (a), la caja es [−L, L] × [−1, 1] × [−1, 1]; para el (b), [−L, L] × 
[0, 1] × [−L, L] en (x, y, z). Calcula el volumen de cada caja.

2. Números aleatorios. Escribe una función double uniforme(double a, double b) que devuelva un número uniforme en 
[a, b) usando rand() y RAND_MAX. Inicializa el generador una sola vez al principio del programa con srand().

3. Conteo. Para cada caso, escribe una función que genere N puntos (x, y, z) dentro de su caja y cuente cuántos cumplen la 
condición de estar dentro del sólido.

4. Estimación. El volumen es el volumen de la caja multiplicado por la fracción de puntos que han caído dentro: 
V ≈ V_caja · (dentro / N).

5. Convergencia. Repite el cálculo para N = 10², 10³, …, 10⁷, calcula el error relativo respecto al
resultado analítico y guarda los resultados en un fichero. Representa el error frente a N en
escala log-log para los dos casos, usando Excel.
*/