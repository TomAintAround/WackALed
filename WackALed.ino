byte minLed = 7;
byte maxLed = 13;
byte buttonPin = 4;
bool ultimoEstadoBotao = LOW;
bool estadoBotaoDebouncing = LOW;
long ultimoDebounce = 0;
long tempoDesdeDebounce = 0;
byte delayDebounce = 50;
byte estadoJogo = 1;

void debouncing() {
    /*
        Contornar o problema do debouncing dos botões
    */

    // Como o pin do botão está em pull up, é necessário inverter a leitura do pin
    bool estadoBotao = !digitalRead(buttonPin);

    // Se o estado do botão for diferente, atualizar o tempo do último debounce.
    if (estadoBotao != ultimoEstadoBotao) ultimoDebounce = millis();
    
    // Atualizar o há quanto tempo o estado do botão foi alterado
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

void preJogo() {
    /*
        Primeira fase do jogo
    */

    // Verificar se o botão está a ser premido mais do que 2 segundos
    if (estadoBotaoDebouncing == HIGH && tempoDesdeDebounce >= 2000) {

        // Ligar e apagar os LEDs a cada segundo. Isto é repetido 3 vezes
        for (byte i = 1; i <= 3; i++) {
            for (byte led = minLed; led <= maxLed; led++) digitalWrite(led, HIGH);
            delay(1000);
            for (byte led = minLed; led <= maxLed; led++) digitalWrite(led, LOW);
            delay(1000);
        }

        // Vai começar o jogo
        estadoJogo++;
    }
}

void jogo() {
    /*
        Segunda fase do jogo
    */
}

void vitoria() {
    /*
        Terceira fase do jogo
    */
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