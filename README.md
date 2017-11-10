# CamFis4
## Projeto 4 de Camada Física da Computação

Este projeto tem o objetivo de implementar uma comunicação serial entre dois dispositivos (Arduino Due) fazendo uso da tecnologia UART

## UART

UART (universal asynchronous receiver-transmitter) se refere a um dispositivo de hardware de emissão e recepção de sinais elétricos arbitrários serializados, com ele podemos definir um protocolo de comunicação entre duas interfaces e enviar informação legível e passível à verificação de erros

A informação é enviada em pulsos de sinais digitais altos e baixos (0 e 1) espaçados em uma quebra de tempo especificada na implementação da interface. Neste projeto executamos os nossos testes usando um baudrate de 9600, ou seja, nossa comunicação é realizada com uma frequência regular, de envio de 9600 bits por segundo.

## Imagens da forma de onda gerada (Analog Discovery)

### Serial monitor printando letra por letra que recebe

<a href="url"><img src="Imagens/print.png" height="400"></a>

### Serial monitor printando com a quebra de linha

<a href="url"><img src="Imagens/print2.png" height="400"></a>

### Analog recebendo dados do arduino

<a href="url"><img src="Imagens/analog.png" lenght="400"></a>

### Sinais podem ser 0 ou 1. No período transitório de sinais, há uma curva que o Analog constrói para representar tal transição.

<a href="url"><img src="Imagens/curva.png" lenght="400"></a>

## Código e Funcionamento

Realizamos o envio de uma mensagem de texto de um dispositivo à outro via um protocolo próprio implementado em UART.

Tratamos o texto enviado por caracteres de tamanho constante de 8 bits codificados em ASCII e delimitado por um caracter não-imprimível de quebra de linha (line feed)

Nossa comunicação é feita em packets, cujo payload é um caracter do texto a ser enviado como seu. O packet inclui também para uma verificação rudimentar de erros um bit de início (precedendo o payload), um bit de paridade e um bit de término (posteriores ao payload), de tal forma que o tamanho total do packet é de 11 bits, sem variação de tamanho.

A mudança de sinal entre o bit de término (valor 1) e o bit de início (valor 0) permite a detecção do início de um novo packet. Os próximos 8 bits que seguem correspondem ao caracter de interesse, que são agrupados respectivamente nos bits de uma variável que corresponderá à forma codificada do caracter original que foi enviado. Em seguida fazemos a checagem de paridade para validar a integridade da informação recebida, fazemos isso checando se a quantia de bits de valor 1 no payload é par ou ímpar, do packet recebemos também um valor par ou ímpar na forma de um bit, o qual é usado para validar nossos resultados; Se a paridade da informação que recebemos for par e a informação que recebemos é de que a informação deveria de fato ser par então validamos a informação, se a paridade for diferente sabemos que houve alguma forma de corrupção na transmissão. Por fim recebemos o bit de término o que indicará para a rotina da nossa implementação se preparar para recomeçar o processo e se preparar para receber um novo packet.
    
Detectamos a mudança de um bit para o outro pela passagem de tempo entre eles, sabemos que nossa comunicação é feita a 9600 bits por segundo. Com isso podemos fazer uso de uma função dedicada à pausar nosso código pela quantidade de tempo que leva 

A implementação do protocolo é separada em dois arquivos principais escritos para Arduino Due, "due_sw_uart_TX.ino" que cuida do envio da informação e "due_sw_uart_RX.ino" que cuida do recebimento
