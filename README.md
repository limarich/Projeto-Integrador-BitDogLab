# Projeto integrador BitdogLab

> A proposta desse projeto é de utilizar os periféricos disponíveis da BitdogLab em conjunto para produzir algum resultado.

---
### Demonstração
![Assista ao Vídeo](https://youtu.be/v2qFn__Rc2U)

Caso o link não funcione copie esse aqui: **https://youtu.be/v2qFn__Rc2U**

---

### Recursos da placa utilizados:

- Buzzers para efeitos sonoros
- Led RGB para efeitos visuais
- Matriz de Led para feedback visual
- Display gráfico (SSD1306) como requisito obrigatório para essa atividade
- Joystick para leitura ADC
- Push buttons para o controle das luzes da placa, com tratamento do debounce
- Comunicação via UART para a leitura dos dados analógicos e I2C para controle do display gráfico
- Interrupções causadas pelos botões
  _Bônus:_
- Uso de Timer para atualização paralela do display gráfico

---

### Funcionalidades implementadas:

> Com a finalidade de criar um projeto que pudesse fazer uso de todos os recursos listados anteriormente, foram implementados os seguintes recursos:

1.  Feedback visual da direção de uso do joystick
    _O usuário ao manusear o joystick pode ver uma seta indicando a direção que o direcional está, além disso um quadrado branco 8x8 aparece no display gráfico seguindo a mesma direção apontada_

2.  Controle das cores da placa com uso dos botões
    _O usuário pode alternar entre as cores disponíveis **(Vermelho, Verde, Azul, Branco)**, pressionando os botões. A cor selecionada por padrão é a vermelha e ao alternar a cor do sistema, tanto a matriz de led, quanto o RGB muda de cor correspondendo à cor escolhida_

3.  Feedback sonoro para indicar mudanças no sistema
    O usuário ao interagir em determinadas ações poderá ser capaz de escutar 3(três) diferentes feedbacks sonoros pelos buzzers.

- **Buzzers**: Emitirão sons distintos para três ações:
- Pressionar botão A (cor anterior)
- Pressionar botão B (próxima cor)
- Pressionar botão do joystick (altera aparência do quadrado)

---

### Processo de instalação e configuração:

> Certifique-se que tenha os seguintes requisitos para testar o programa:

- Tenha instalado o **VScode**
- Tenha A extensão Raspberry Pi Pico
- Importado corretamente o projeto através da extensão, caso contrário o CMAKE pode ser não reconhecido
