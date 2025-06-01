import random

# Leer los IDs de cuentas desde usuarios.csv
cuentas = []
monedas = []
with open("src/usuarios.csv", "r", encoding="utf-8") as f:
    for linea in f:
        partes = linea.strip().split(",")
        if len(partes) >= 2:
            cuenta = partes[1].strip()
            if cuenta.isdigit():
                cuentas.append(cuenta)

with open("src/criptomonedas.csv", "r", encoding="utf-8") as f:
    for linea in f:
        partes = linea.strip().split(",")
        if len(partes) >= 2:
            simbolo = partes[1].strip()
            if simbolo:
                monedas.append(simbolo)                

# Generar 1000 transacciones aleatorias
with open("src/transacciones_transferencias.csv", "w", encoding="utf-8") as f:
    for _ in range(1000):
        emisor, receptor = random.sample(cuentas, 2)
        monto = round(random.uniform(1000, 100000), 2)
        f.write(f"{emisor},{receptor},{monto:.2f}\n")

# Generar 1000 transacciones aleatorias de criptomonedas
with open("src/transacciones_criptomonedas.csv", "w", encoding="utf-8") as f:
    for _ in range(1000):
        comprador = random.choice(cuentas)
        moneda = random.choice(monedas)
        monto = round(random.uniform(0.01, 10), 2)
        f.write(f"{comprador},{moneda},{monto:.2f}\n")