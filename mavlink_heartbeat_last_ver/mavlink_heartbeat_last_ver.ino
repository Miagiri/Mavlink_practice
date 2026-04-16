#include "mavlink/minimal/mavlink.h"  // Подключаем библиотеку MAVLink для работы с протоколом

void setup() {
  Serial.begin(19200);  // Инициализируем последовательный порт для связи с полетным контроллером на скорости 19200 бод
}

void loop() {
  static int heartbeatCounter = 0;           // Статическая переменная-счетчик heartbeat'ов (сохраняет значение между вызовами loop)
  static unsigned long lastHeartbeatTime = 0; // Время последнего полученного heartbeat в миллисекундах
  
  mavlink_message_t msg;      // Структура для хранения принятого MAVLink сообщения
  mavlink_status_t status;    // Структура для хранения статуса парсинга MAVLink
  
  while (Serial.available()) {  // Пока в буфере UART есть непрочитанные байты
    // Парсим принятый байт. Если сформировано полное сообщение - функция возвращает true
    if (mavlink_parse_char(MAVLINK_COMM_0, Serial.read(), &msg, &status)) {
      
      // Проверяем, является ли полученное сообщение heartbeat (ID = 0)
      if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
        
        heartbeatCounter++;                           // Увеличиваем счетчик на 1
        unsigned long currentTime = millis();         // Получаем текущее время в миллисекундах с момента старта Arduino
        
        // Вычисляем интервал с момента последнего heartbeat (в секундах)
        float interval = 0.0;
        if (lastHeartbeatTime != 0) {                 // Если это не первый heartbeat
          interval = (currentTime - lastHeartbeatTime) / 1000.0;  // Переводим миллисекунды в секунды
        }
        
        // Выводим информацию в монитор порта
        Serial.print("Heartbeat #");          // Выводим текст "Heartbeat #"
        Serial.print(heartbeatCounter);       // Выводим номер heartbeat по счету
        Serial.print(" | Time: ");            // Выводим разделитель и текст "Time: "
        Serial.print(currentTime / 1000.0);   // Выводим время в секундах от старта (с плавающей точкой)
        Serial.print(" sec");                 // Выводим единицу измерения
        
        if (interval > 0) {                   // Если интервал известен
          Serial.print(" | Interval: ");      // Выводим текст "Interval: "
          Serial.print(interval);             // Выводим интервал между heartbeat'ами
          Serial.print(" sec");               // Выводим единицу измерения
        }
        
        Serial.println();                     // Переходим на новую строку
        
        lastHeartbeatTime = currentTime;      // Запоминаем время этого heartbeat для следующего расчета
      }
    }
  }
}
