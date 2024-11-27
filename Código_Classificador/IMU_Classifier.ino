// CÓDIGO PARA TESTE DO MODELO DE MACHINE LEARNING DESENVOLVIDO EM PYTHON PARA INTERPRETAR GESTOS DE SOCO E FLEXÃO DO BRAÇO //

// -------------------------------------------------------------------------------------------------------------------------

#include <Arduino_LSM9DS1.h>                    // Biblioteca para o sensor IMU LSM9DS1
#include <ArduTFLite.h>                         // Biblioteca para usar modelos TensorFlow Lite no Arduino

//#include "DNN_model_Adam.h"                     // Descomente para usar o modelo Adam
#include "DNN_model_SGD.h"                      // Descomente para usar o modelo SGD

// -------------------------------------------------------------------------------------------------------------------------

const float accelerationThreshold = 3;          // Limiar de aceleração para iniciar a coleta de amostras
const int numSamples = 200;                     // Número de amostras a serem coletadas

int samplesRead;                                // Contador para rastrear o número de amostras lidas
const int inputLength = numSamples * 6;         // Comprimento da entrada para o modelo

constexpr int tensorArenaSize = 8 * 1024;       // Tamanho da área de memória reservada para o TensorFlow Lite.
alignas(16) byte tensorArena[tensorArenaSize];  // Área de memória alocada para o TensorFlow Lite

const char* GESTURES[] =                        // Cria o vetor com as Labels que a I.A. vai ter que dividir os resultados
{
  "Soco",
  "Flexao",
  "Direita",
  "Esquerda"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0])) // Calcula o número total de gestos

// -------------------------------------------------------------------------------------------------------------------------

void setup() 
{
  Serial.begin(9600);                                           // Inicializa a comunicação serial com uma taxa de 9600 bauds
  while (!Serial);                                              // Aguarda até que a comunicação serial esteja pronta

  if (!IMU.begin())                                             // Tenta inicializar o sensor IMU e se falhar, 
  {
    Serial.println("Inicialização do sensor IMU falhou!");      // imprime uma mensagem
    while(true);                                                // e entra em um loop infinito
  }

  Serial.print("Frequências de amostragem do acelerômetro = "); // Imprimem as frequências de amostragem do acelerômetro
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.print("Frequências de amostragem do giroscópio = ");   // Imprimem as frequências de amostragem do giroscópio
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.println();
  Serial.println("Iniciando o modelo...");                      // Printa que será iniciado o modelo
  
  if (!modelInit(model, tensorArena, tensorArenaSize))          // Inicializa o modelo TensorFlowLite com a área de memória e o tamanho da área, e se falhar,
  {
    Serial.println("Inicialização do modelo falhou!");          // imprime uma mensagem
    while(true);                                                // e entra em um loop infinito
  }
  Serial.println("Inicialização do modelo feita com sucesso."); // Se não falhar, printa que conectou com sucesso
}

// -------------------------------------------------------------------------------------------------------------------------

void loop() 
{
  float aX, aY, aZ, gX, gY, gZ;

  while (true)                                                      // Loop infinito que aguarda até que o movimento significativo seja detectado
  {
    if (IMU.accelerationAvailable())                                // Verifica se os dados de aceleração estão disponíveis
    {
      IMU.readAcceleration(aX, aY, aZ);                             // Lê os valores de aceleração nos eixos X, Y e Z
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);                  // Calcula a soma das magnitudes absolutas das acelerações

      if (aSum >= accelerationThreshold)                            // Verifica se a soma das acelerações é maior ou igual ao limiar 
      {
        samplesRead = 0;                                            // Se for, reseta o contador de amostras
        break;                                                      // e sai do laço
      }
    }
  }

  while (samplesRead < numSamples)                                  // Coleta dados até que o número especificado de amostras seja alcançado
  {
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable())    // Verifica se os dados de aceleração e giroscópio estão disponíveis
    {
      IMU.readAcceleration(aX, aY, aZ);                             // Lê os dados de aceleração
      IMU.readGyroscope(gX, gY, gZ);                                // Lê os dados de giroscópio
      
      aX = (aX + 4.0) / 8.0;                                        // Normaliza os valores de aceleração e de giroscópio no intervalo 0 e 1
      aY = (aY + 4.0) / 8.0;
      aZ = (aZ + 4.0) / 8.0;
      gX = (gX + 2000.0) / 4000.0;
      gY = (gY + 2000.0) / 4000.0;
      gZ = (gZ + 2000.0) / 4000.0;
      
      modelSetInput(aX,samplesRead * 6 + 0);                        // Define os dados normalizados como entrada para o modelo
      modelSetInput(aY,samplesRead * 6 + 1);
      modelSetInput(aZ,samplesRead * 6 + 2); 
      modelSetInput(gX,samplesRead * 6 + 3);
      modelSetInput(gY,samplesRead * 6 + 4);
      modelSetInput(gZ,samplesRead * 6 + 5); 
      
      samplesRead++;                                                // Incrementa o contador de amostras lidas
      
      if(samplesRead == numSamples)                                 // Quando o número necessário de amostras for coletado,
      {
        if(!modelRunInference())                                    // executa a inferência do modelo e se falhar,
        {
          Serial.println("Inferência do modelo falhou!");           // imprime uma mensagem de erro e retorna
          return;
        }

        for(int i = 0; i < NUM_GESTURES; i++)                       // Obtém a probabilidade para cada gesto e o resultado é convertido para % e é plotado
        {
          Serial.print(GESTURES[i]);
          Serial.print(": ");
          Serial.print(modelGetOutput(i)*100, 2);
          Serial.println("%");
        }
        
        Serial.println();
      }
    }
  }
}
