#include <Servo.h>
// Объявление класса
class Flasher
{
  // Переменные - члены класса 
  // Инициализируются при запуске
  int ledPin; // НОМЕР ПИНА
  int ledState; // Текущее состояние пина
  long  onTime; // временной интервал работы (вкл)
  long  offTime; // временной интервал паузы (выкл)
  
  unsigned long currentMillis; // Текущее состояние счетчика времени
  unsigned long prevMillis; // Предыдущее состояние счетчика времени


  // Конструктор создает экземпляр класса Flasher
  // и инициализирует переменные-члены класса и состояние
  public:
  Flasher(int pin, long on, long off)
  {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
    
    onTime = on;
    offTime = off;
    
    ledState = LOW;
    prevMillis = 0;
    currentMillis = 0;
  }

  void Update() {
    // put your main code here, to run repeatedly:
    currentMillis = millis();
    if ((ledState == HIGH) && (currentMillis - prevMillis >= onTime)){
      // Запоминаем момент времени для последующего сравнения
      prevMillis = currentMillis;
      // Изменяяем сщстояние ledState
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    if ((ledState == LOW) && (currentMillis - prevMillis >= offTime)){
      // Запоминаем момент времени для последующего сравнения
      prevMillis = currentMillis;
      // Изменяяем сщстояние ledState
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
  }
};


int servoPin = 9;  // Управляющий пин серво
int servoKeyPin = 4;  // Пин транзистора включающего серво
int pwmVol;  // Величина ПВМ сигнала
int servoDefPos = 2;  // Исходная позиция серво
int buttonPin = 3;  // Пин кнопки 
int ledKeyPin = 10;  // Пин транзистора подсветки кнопки
int orangeLed1 = 12;
int orangeLed2 = 13;
Servo servo;

// Создаем экземпляры класса led1 led2 и передаем параметры
Flasher led1(orangeLed1, 55, 250);
Flasher led2(orangeLed2, 50, 300);


void setup() {
  Serial.begin(9600);
  pinMode(servoPin, OUTPUT);
  pinMode(servoKeyPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledKeyPin, OUTPUT);
  // Плавно без рывков запускем серво
  // Закрываем транзистро питающий серво
  digitalWrite(servoKeyPin, LOW);
  // Готовимся подать на упр вывод сигнал
  servo.write(servoDefPos);
  // Цепляем серво на соотв вывод
  servo.attach(servoPin);
  delay(100);
  // Плавно открываем транзистор питания (управляющий сигнал с правильной позицией уже ждет на соотв выводе)
  writePWM(0, 255, 5);
  // На всякий случай читаем реальню позицию серво и плавно приводим ее в исходное положение 
  servoMove(servo.read(), servoDefPos, 5);
  // Отключаем серво от управляющего вывода что бы не вибрировала :)
  servo.detach();
  // Все подготовили - включаем подсветку кнопки - режим готовности
  digitalWrite(ledKeyPin, HIGH);  
}

void loop() {
  // Ждем нажати кнопки  
  if (digitalRead(buttonPin) == HIGH){
    // Кнопка нажата - двигаем машину и мигаем оранж. огнями (мигание в цикле сервы и в паузе (см. функции))
    
    // Гасим подсветку кнопки
    digitalWrite(ledKeyPin, LOW);
    
    // Едем вперед
    servoMove(servoDefPos, 70, 10);
     // Пауза с миганием
    //timeDelay(10);

     // Откат
    servoMove(70, servoDefPos, 10);
     // Пауза с миганием
    //timeDelay(10);

     // Едем вперед
    servoMove(servoDefPos, 140, 10);
     // Пауза с миганием
    timeDelay(300);

    // Едем назад
    servoMove(140, servoDefPos, 10);
    timeDelay(500);
    
    // Все сделали - 
    // включаем обратно подсветку кнопки
    digitalWrite(ledKeyPin, HIGH);
    // На всякий случай гасим обе мигалки
    digitalWrite(orangeLed1, LOW);
    digitalWrite(orangeLed2, LOW);
  }
  

}
// Slow PWM writing function
void writePWM (int pwmFrom, int pwmTo, int delayTime){
  int k;
  if (pwmFrom < pwmTo){
    k = 1;
  } else {
    k = -1;
  }
  for (pwmVol = pwmFrom; pwmVol != pwmTo; pwmVol = pwmVol + k){
    analogWrite(servoKeyPin, pwmVol);
    delay(delayTime);
  }
}
// Servo moving function
void servoMove(int posFrom, int posTo, int delayTime){
  servo.attach(servoPin);
  int k;
  if (posFrom < posTo){
    k = 1;  
  } else {
    k = -1;
  }
  for (int pos = posFrom; pos != posTo; pos = pos + k){
    servo.write(pos);
    delay(delayTime);
    led1.Update();
    led2.Update();
  }
  servo.detach();
}

// Pause with blinking function
void timeDelay(int millsDelay){
  for(int tim = 0; tim < 500; tim++){
    Serial.println(tim);
    led1.Update();
    led2.Update();
    }

}
    
