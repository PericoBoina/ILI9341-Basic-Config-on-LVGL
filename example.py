import serial
import time

# Configuración del puerto serial (COM9, 115200 baudios)
puerto = 'COM9'
baud_rate = 115200

# Abrir puerto serial
ser = serial.Serial(puerto, baud_rate)

# Función para enviar un valor por el puerto serial
def enviar_dato(valor):
    ser.write(f"{valor}\n".encode())  # Enviar el valor como cadena seguida de salto de línea
    print(f"Enviado: {valor}")
    time.sleep(0.5)  # Esperar medio segundo entre envíos

# Enviar valores ascendentes de 0 a 100 en saltos de 10
for i in range(0, 101, 10):
    enviar_dato(i)

# Enviar valores descendentes de 100 a 0 en saltos de 10
for i in range(100, -1, -10):
    enviar_dato(i)

# Cerrar el puerto serial
ser.close()
