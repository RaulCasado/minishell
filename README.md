⭐️ **Minishell - Parte Obligatoria**

---

## 🛠️ **Parte Obligatoria**

### 🔹 **Estructura del Proyecto**
- **Nombre del programa:** `minishell`
- **Archivos a entregar:** `Makefile`, `*.h`, `*.c`
- **Funciones autorizadas:**
  - **Manejo de entrada/salida:** `readline`, `printf`, `write`, `read`, `open`, `close`, etc.
  - **Manejo de procesos:** `fork`, `wait`, `execve`, `pipe`, `kill`, etc.
  - **Señales:** `signal`, `sigaction`, etc.
  - **Otras:** `getcwd`, `chdir`, `opendir`, `readdir`, `isatty`, `getenv`, etc.

---

### 🔹 **Características del Shell**

#### ✅ **Interfaz y Comportamiento**
- El shell debe **mostrar un prompt** mientras espera la entrada de un comando.
- Debe tener un **historial funcional** mediante `readline` y `add_history`.
- Debe **buscar y ejecutar el ejecutable correcto**:
  - Usando la variable `$PATH`
  - Mediante rutas absolutas (`/bin/ls`)
  - Mediante rutas relativas (`./a.out`)

#### 🚨 **Restricciones**
- Solo se **permite una variable global**, que **únicamente** puede indicar la recepción de una señal.
- **No debe interpretar comillas sin cerrar** o caracteres especiales no especificados.

#### 📝 **Gestión de Comillas**
- **Comillas simples (`'`)** → Evitan que el shell interprete los metacaracteres en su interior.
- **Comillas dobles (`"`)** → Evitan que el shell interprete metacaracteres, excepto `$` (expansión de variables de entorno).

#### 🔀 **Redirecciones**
- **`<`** → Redirige la entrada estándar desde un archivo.
- **`>`** → Redirige la salida estándar a un archivo (sobrescribiendo su contenido).
- **`<<`** (Heredoc) → Permite escribir múltiples líneas hasta encontrar un delimitador específico (no debe guardarse en el historial).
- **`>>`** → Redirige la salida estándar a un archivo en modo `append`.

#### 🔗 **Pipes (`|`)**
- Permiten **encadenar comandos**, donde la salida de un comando se convierte en la entrada del siguiente.

#### 🌍 **Expansión de Variables de Entorno**
- **`$VAR`** → Debe expandirse al valor de la variable de entorno correspondiente.
- **`$?`** → Se expande al código de salida del último comando ejecutado.

#### ⌨️ **Gestión de Señales (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)**
- **`Ctrl+C`** → Imprime una nueva línea con el prompt sin salir del shell.
- **`Ctrl+D`** → Finaliza el shell.
- **`Ctrl+\`** → No debe hacer nada.

#### 🔧 **Built-ins requeridos**
El Minishell debe implementar los siguientes comandos internos:
- **`echo`** → Soporte para la opción `-n` (sin salto de línea final).
- **`cd`** → Permite cambiar de directorio (solo con rutas absolutas o relativas).
- **`pwd`** → Muestra el directorio actual (sin opciones).
- **`export`** → Permite definir variables de entorno (sin opciones).
- **`unset`** → Permite eliminar variables de entorno (sin opciones).
- **`env`** → Muestra las variables de entorno actuales (sin opciones o argumentos).
- **`exit`** → Cierra el shell.

---

### 📌 **Notas Finales**
- **No es necesario corregir los memory leaks generados por `readline`.**
- **El código escrito por ti sí debe estar libre de memory leaks.**
- **Bash puede usarse como referencia para el comportamiento esperado.**
- **No implementes características extra que no se pidan en el enunciado.**

---