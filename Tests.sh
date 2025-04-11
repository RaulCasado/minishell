!bin/bash

# valgrind --leak-check=full --suppressions=readline.supp -s ./minishell
# clear && make && valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp -s ./minishell
	echo 		hola $? '$USER'	

# Test fallido
Test  65: ❌ echo <"./test_files/infile" <missing <"./test_files/infile" 
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( No such file or directory)

# Terminal normal
c3r3s6% echo <"./test_files/infile" <missing <"./test_files/infile"
zsh: no such file or directory: missing
c3r3s6% echo $?
1

# Funciona no????
Minishell> echo <"./test_files/infile" <missing <"./test_files/infile"
Minishell: ./test_files/infile: No such file or directory
Minishell> echo $?
1

#
echo " $USER"
echo ' $USER'

# 
echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
echo hola "$USER $?" adios
echo hola ""    "" hola
echo hola ""    ccalcio""hola
echo "hola $USER" "hola $?" '$?' $?
echo "$USER $? '$USER'"
echo "$? hola que tal"$USER""
echo "Hello world"
echo 'Hello world'
echo "Hello 'world'"
echo 'Hello "world"'
echo "Hello (comilla sin cerrar)
echo 'Hello (comilla sin cerrar)
echo " Hello world "
echo Hello   world
echo "*"
echo "$USER"
echo '$USER'
echo "hola $USER" "hola $?" '$?' $?

#
echo -n Hello
echo -nnnnnn Hello
echo -nnmnnn Hello
echo -n "Texto sin salto de línea"
echo "Texto con salto de línea"
echo -n "$USER"  # Muestra el valor de USER sin salto de línea
echo -n "$USER $PWD"  # Combinación de variables sin salto de línea

# CD
cd ..
echo "Directorio después de cd ..: $PWD"
cd /tmp
echo "Directorio después de cd /tmp: $PWD"
cd nonexistentdir 2>/dev/null || echo "Error al acceder a nonexistentdir"
cd -  # Volver al directorio anterior

# PWD
pwd

# EXPORT
export AZUL="blue"
export NAME="Minishell"
echo "$AZUL"
echo "$NAME"

# UNSET
unset AZUL
echo "$AZUL"  # No debería mostrar nada

# ENV
env  # Mostrar todas las variables de entorno

# EXIT
exit  # Esto cerrará la minishell

# Redirección de salida (>)
echo "Texto redirigido" > output.txt
cat output.txt

# Redirección de salida en modo append (>>)
echo "Texto adicional" >> output.txt
cat output.txt

# Redirección de entrada (<)
echo "Hola desde input.txt" > input.txt
cat < input.txt

# Heredoc (<<)
cat << EOF
Texto pasado al comando cat con heredoc.
EOF

# Pipe (|)
echo "Prueba pipe" | grep "Prueba"

# Redirección de error (2>)
ls nonexistentdir 2> error.log
cat error.log  # Debería mostrar el error

# LS con redirección
ls /tmp > listado.txt
cat listado.txt

# LS con pipe
ls /tmp | grep "log"

# Comando externo
ls /bin

# Comando no encontrado
nonexistentcommand 2>/dev/null  # Error silenciado
