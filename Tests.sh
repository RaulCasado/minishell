!bin/bash

# Cuando se ejecuta la primera vez y AZUL no existe ==> Invalid read de 1 byte en Forge_value
echo $AZUL

# Cuando hay salto de línea no se borran bien las comillas finales
# propongo borrar los saltos de línea
echo " $USER"


#El otro minishell no pone los saltos de línea como el shell normal que los mantiene,
#Minishell> echo "$? hola que tal"$USER""		<- Normal
#0 hola que taldanielr
#Minishell> echo "$? hola que tal"$USER""		<- Salto de línea después de "
#0 hola que taldanielr"

# 
echo "$USER" "$-HOLA" "$1ERROR" "$ ERROR" $ERROR
echo hola "$USER $?" adios

echo hola ""    "" hola

echo hola ""    ccalcio""hola
bash: hola  ccalciohola
mini: hola  ccalcio  hola

echo "Hello world"
bash: Hello world


echo "hola $USER" "hola $?" '$?' $?
echo "$USER $? '$USER'"
echo "$? hola que tal"$USER""

🔹 Pruebas con comillas
echo "Hello world"
echo 'Hello world'
echo "Hello 'world'"
echo 'Hello "world"'
echo "Hello (comilla sin cerrar)
echo 'Hello (comilla sin cerrar)


🔹 Pruebas con espacios y caracteres especiales

echo " Hello world "
echo Hello   world
echo "*"
echo "$USER"
echo '$USER'

🔹 Pruebas con -n
echo -n Hello
echo -nnnnnn Hello




Minishell> echo "hola $USER" "hola $?" '$?' $?
hola droura-s hola 0 $? 0

HEAP SUMMARY:
    in use at exit: 208,189 bytes in 226 blocks
  total heap usage: 691 allocs, 465 frees, 246,250 bytes allocated

8 bytes in 4 blocks are definitely lost in loss record 3 of 64
   at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x407717: ft_itoa (in /home/droura-s/Minichell/minishell/minishell)
   by 0x402CC6: get_variable (in /home/droura-s/Minichell/minishell/minishell)
   by 0x402B2C: expand_variable (in /home/droura-s/Minichell/minishell/minishell)
   by 0x402ABB: expand_loop (in /home/droura-s/Minichell/minishell/minishell)
   by 0x402921: expand_tokens (in /home/droura-s/Minichell/minishell/minishell)
   by 0x402426: tokenize_input (in /home/droura-s/Minichell/minishell/minishell)
   by 0x401450: process_iteration (in /home/droura-s/Minichell/minishell/minishell)
   by 0x4013A1: minishell_loop (in /home/droura-s/Minichell/minishell/minishell)
   by 0x40133C: main (in /home/droura-s/Minichell/minishell/minishell)

LEAK SUMMARY:
   definitely lost: 8 bytes in 4 blocks
   indirectly lost: 0 bytes in 0 blocks

ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)