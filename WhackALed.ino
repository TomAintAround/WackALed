/*
    Materiais:
     * 1 Arduino Uno
     * 1 breadboard
     * 1 botão
     * 7 LEDs
     * 7 resistores de 220 ohms
     * 11 fios
*/

byte minLed = 7;
byte maxLed = 13;
byte buttonPin = 4;
bool ultimoEstadoBotao = LOW;
bool estadoBotaoDebouncing = LOW;
long ultimoDebounce = 0;
long tempoDesdeDebounce = 0;
byte delayDebounce = 50;
byte estadoJogo = 1;
byte pontuacao = 0;
long tempoMarcado = 0;

void debouncing() {
    /*
        Contornar o problema do bouncing dos botões
    */

    // Como o pin do botão está em pull up, é necessário inverter a leitura do pin
    bool estadoBotao = !digitalRead(buttonPin);

    // Se o estado do botão for diferente, atualizar o tempo do último debounce.
    if (estadoBotao != ultimoEstadoBotao) ultimoDebounce = millis();
    
    // Atualizar há quanto tempo o estado do botão foi alterado
    tempoDesdeDebounce = millis() - ultimoDebounce;

    // Executar se o tempo atual menos o do último debounce for maior que a margem definida
    if (tempoDesdeDebounce > delayDebounce) {

        // Atualizar o estado do botão
        if (estadoBotaoDebouncing != estadoBotao) {
            estadoBotaoDebouncing = estadoBotao;
        }
    }

    ultimoEstadoBotao = estadoBotao;
}

void piscarLEDs(byte repeticoes) {
    /*
        Ligar e desligar os LEDs a cada segundo em um determinado número de vezes
    */

    for (byte i = 1; i <= repeticoes; i++) {
        for (byte led = minLed; led <= maxLed; led++) digitalWrite(led, HIGH);
        delay(1000);
        for (byte led = minLed; led <= maxLed; led++) digitalWrite(led, LOW);
        delay(1000);
    }
}

void preJogo() {
    /*
        Primeira fase do jogo
    */

    // Verificar se o botão está a ser premido mais do que 2 segundos
    if (estadoBotaoDebouncing == HIGH && tempoDesdeDebounce >= 2000) {
        piscarLEDs(3);

        // Começar o jogo
        estadoJogo++;

        /*
            Desativar o botão. Caso isto não seja feito,
            o jogo iniciará logo com o botão ativado e
            alguns LEDs serão ativados logo no início.
            Isto acontece graças aos delays usados acima
            e achei mais fácil contornar este erro assim.
        */
        estadoBotaoDebouncing = 0;
    }
}

void jogo() {
    /*
        Segunda fase do jogo
    */

    /*
        Esta variável é atualizada dentro da condição abaixo.
        Por isso, para ser usada fora do scope da condição,
        esta precisa de ser declarada fora dela.
    */
    byte ledsLigados;

    // Executar a cada 250 milissegundos
    if (millis() - tempoMarcado >= 250) {

        // Transformar 1 bit aleatório na pontuação em 1, caso ainda não tenha ocorrido
        ledsLigados = (1 << random(0, maxLed - minLed + 1)) | pontuacao;

        // Ligar os LEDs de acordo com a variável ledsLigados
        for (byte led = minLed; led <= maxLed; led++) {
            bool ledLigado = ledsLigados >> (led - minLed) & 1;
            digitalWrite(led, ledLigado);
        }

        // Fazer reset ao timer de 250 milissegundos
        tempoMarcado = millis();
    }

    // Guardar pontuação caso seja premido o botão rapidamente
    if (estadoBotaoDebouncing == HIGH && tempoDesdeDebounce <= 150) pontuacao = ledsLigados;

    // Reiniciar o jogo se o botão for premido num período de 2 segundos ou mais
    if (estadoBotaoDebouncing == HIGH && tempoDesdeDebounce >= 2000) {
        piscarLEDs(2);
        pontuacao = 0;
        estadoJogo--; // Voltar ao preJogo
    }

    /*
        Quando todos os LEDs acenderem, ir para a fase da vitória.
        Como se tratam de 7 leds, os LEDS estarem todos acesos é
        equivalente a pontuação ser 0b10000000 - 1 = 0b1111111.
    */
    if (pontuacao == (1 << maxLed - minLed + 1) - 1) estadoJogo++;
}

void vitoria() {
    /*
        Terceira fase do jogo
    */

    piscarLEDs(3);
    pontuacao = 0;
    estadoJogo = 1; // Voltar ao princípio
}

void acaoAtual() {
    /*
        Escolher a fase do jogo
        A variável estadoJogo começa a 1
    */

    if (estadoJogo == 1) preJogo();
    else if (estadoJogo == 2) jogo();
    else if (estadoJogo == 3) vitoria();
}

void setup() {
    // Útil para debugging
    Serial.begin(9600);

    // Configurar as portas I/O do Arduino para os LEDs e o botão
    for (byte led = minLed; led <= maxLed; led++) pinMode(led, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
    debouncing();
    acaoAtual();
}
