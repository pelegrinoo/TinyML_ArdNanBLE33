// CÓDIGO DE CAPTAÇÃO DE VALORES DE UM SENSOR IMU PARA TESTES COM DADOS DE ACELERÔMETRO E GIRSCÓPIO //

// ---------------------------------------------------------------------------------------------------

#include <Arduino_LSM9DS1.h> // Fornece funções para acessar o sensor de aceleração e giroscópio LSM9DS1

// ---------------------------------------------------------------------------------------------------

const float accelerationThreshold = 3;   // Define um valor mínimo de aceleração para captar os dados
const int numSamples = 200;              // Defnie o número de dados que serão agrupados após detectar movimento significativo
int samplesRead = numSamples;            // Garante que o primeiro laço inicie só quando detectar movimento significativo

// ---------------------------------------------------------------------------------------------------

void setup()                                        // Função de setup para o processo ocorrer
{
  Serial.begin(9600);                               // Inicializa a comunicação serial com uma taxa de 9600 bauds (bits/s)
  while(!Serial);                                   // Aguarda até que a comunicação serial esteja pronta
  
  if (!IMU.begin()){                                // Tenta iniciar o sensor IMU,
    Serial.println("Failed to initialize IMU!");    // se falhar, imprime uma mensagem de erro
    while(1);                                       // e entra em um loop infinito
  }

  Serial.println("aX,aY,aZ,gX,gY,gZ");              // Imprime um cabeçalho para os dados que serão enviados para a serial
}

// ---------------------------------------------------------------------------------------------------

void loop()                                         // Função de loop do processo 
{
  float aX, aY, aZ, gX, gY, gZ;                     // Declara variáveis para armazenar os dados de aceleração e giroscópio

  while (samplesRead == numSamples)                 // Espera até que um movimento significativo seja detectado
  {
    if (IMU.accelerationAvailable())                // Verifica se há dados de aceleração disponíveis para leitura
    {
      IMU.readAcceleration(aX, aY, aZ);             // Lê os valores de aceleração nos eixos XYZ e os armazena nas variáveis aX, aY e aZ
      
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);  // Calcula a soma das magnitudes absolutas das acelerações nos três eixos, 
                                                    // e isso é feito para determinar se o movimento é significativo, independentemente da direção

      if (aSum >= accelerationThreshold)            // Verifica se a soma das magnitudes é maior ou igual ao limiar de aceleração definido
      {
        samplesRead = 0;                            // Reseta o contador de amostras lidas para começar a coleta de novas amostras
        break;                                      // Sai do laço while após detectar um movimento significativo
      }
    }
  }

  while (samplesRead < numSamples)                                // Coleta e imprime amostras de dados até o número especificado de amostras
  {
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable())  // Verifica se os dados de aceleração e de giroscópio estão disponíveis para leitura
    {
      IMU.readAcceleration(aX, aY, aZ);                           // Lê os dados de aceleração e armazena nas variáveis aX, aY, e aZ
      IMU.readGyroscope(gX, gY, gZ);                              // Lê os dados de giroscópio e armazena nas variáveis gX, gY, e gZ

      samplesRead++;                                              // Incrementa o contador de amostras lidas.

      Serial.print(aX, 3);                                        // Imprime os dados de aceleração e giroscópio no formato .CSV com três casas decimais
      Serial.print(',');
      Serial.print(aY, 3);
      Serial.print(',');
      Serial.print(aZ, 3);
      Serial.print(',');
      Serial.print(gX, 3);
      Serial.print(',');
      Serial.print(gY, 3);
      Serial.print(',');
      Serial.print(gZ, 3);
      Serial.println();

      if (samplesRead == numSamples)                              // Adiciona uma linha em branco no final da coleta de amostras, 
      {                                                           // se o número de amostras for igual a numSamples
        Serial.println();
      }
    }
  }
}
