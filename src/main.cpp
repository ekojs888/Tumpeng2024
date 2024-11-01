#include <Arduino.h>

// Definisikan pin untuk setiap driver stepper (sesuaikan dengan pin pada rangkaian)
#define NAIK true
#define TURUN false

#if defined(ESP32)
#define STEPPER_EN 26
#define STEPPER1_STEP_PIN 25
#define STEPPER1_DIR_PIN 17
#elif defined(ARDUINO_ARCH_AVR)
#define STEPPER_EN 2
#define STEPPER1_STEP_PIN 3
#define STEPPER1_DIR_PIN 4
#else
    #error "Platform tidak didukung. Harap gunakan ESP32 atau Arduino Uno."
#endif
// Jumlah langkah per putaran penuh untuk stepper (misalnya 200 untuk stepper 1,8° per langkah)
#define STEPS_PER_REVOLUTION 3200L

// Kelas untuk kontrol motor stepper dengan pengaturan kecepatan RPM
class StepperController
{
private:
  int stepPin, dirPin;
  long stepsRemaining;
  unsigned long lastStepTime;
  unsigned long stepInterval; // Interval waktu antar langkah dalam mikrodetik

public:
  // Konstruktor untuk inisialisasi pin stepper dan variabel lainnya
  StepperController(int stepPin, int dirPin)
      : stepPin(stepPin), dirPin(dirPin), stepsRemaining(0), lastStepTime(0), stepInterval(1000)
  {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(STEPPER_EN, OUTPUT);
    digitalWrite(STEPPER_EN, LOW);
  }

  // Mengatur kecepatan dalam RPM
  void setRPM(float rpm)
  {
    // Menghitung waktu antar langkah berdasarkan RPM
    stepInterval = 60000000L / (STEPS_PER_REVOLUTION * rpm);
  }

  // Memulai pergerakan motor untuk sejumlah langkah tertentu dan arah
  void move(long steps, bool clockwise)
  {
    digitalWrite(dirPin, clockwise ? HIGH : LOW);
    stepsRemaining = steps;
  }

  // Panggil fungsi ini dalam loop untuk memperbarui posisi stepper secara non-blocking
  void update()
  {
    if (stepsRemaining > 0)
    {
      unsigned long currentTime = micros();
      if (currentTime - lastStepTime >= stepInterval)
      {
        lastStepTime = currentTime;
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1); // Pulsa pendek untuk setiap langkah
        digitalWrite(stepPin, LOW);
        stepsRemaining--;
      }
    }
  }

  // Mengecek apakah motor masih bergerak
  bool isMoving()
  {
    return stepsRemaining > 0;
  }
};

// Membuat objek untuk masing-masing stepper (6 stepper)
StepperController stepper1(STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);

// Variabel untuk status pergerakan
bool isOpening = true; // Status untuk membuka kelopak

void setup()
{
  // Inisialisasi kecepatan dalam RPM untuk setiap stepper
  stepper1.setRPM(350); // Sesuaikan RPM untuk kecepatan yang diinginkan

  // Mulai gerakan membuka untuk setiap kelopak
  long stepsToOpen = STEPS_PER_REVOLUTION *1; // Jumlah langkah untuk membuka kelopak
  stepper1.move(stepsToOpen, NAIK);
}

void loop()
{
  // Memperbarui posisi setiap motor stepper dalam setiap iterasi loop
  stepper1.update();

  // Cek apakah semua motor selesai membuka kelopak
  if (isOpening && !stepper1.isMoving())
  {
    // Set status menjadi false untuk menandakan bahwa semua kelopak telah terbuka
    isOpening = false;
  }
}
