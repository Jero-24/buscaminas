
# 🕹️ Buscaminas en C (versión consola)

Un clásico **Buscaminas** implementado en **C**, jugable directamente en la terminal.  
El proyecto está organizado de forma modular, separando la lógica del tablero, la interfaz y las utilidades.

## 🎮 Características

- Tablero jugable en la terminal con tamaño y dificultad personalizables. 
- Colores. 
- Fácil de compilar y ejecutar en sistemas con **GCC**.

## 🛠️ Requisitos

- **GCC** (o cualquier compilador de C compatible)  
- Terminal o consola de comandos (Linux, macOS, Windows con WSL o MinGW)

## 🚀 Instalación y ejecución

Clona este repositorio:

```bash
git clone https://github.com/Jero-24/buscaminas.git
```

### Versión estática

```bash
cd buscaminas/BuscaminasEstatico
gcc buscaminas.c funciones/*.c -o buscaminas
./buscaminas
```

### Versión dinámica (experimental)

```bash
cd buscaminas/BuscaminasDinamico
gcc buscaminas_dinamico.c funciones/*.c -o buscaminas_dinamico
./buscaminas_dinamico
```

## 🤝 Contribuciones

Pull requests y sugerencias son bienvenidas.
Si encuentras un error o quieres mejorar el proyecto, ¡no dudes en contribuir!

## ⚖️ Licencia

Este proyecto está bajo licencia **MIT**.
