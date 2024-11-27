---

## **🚀 Implementação de I.A. no Arduino Nano BLE 33**  
### *Identificação de Quatro Gestos*  

---

### **📖 Sobre o Projeto**  
Uma solução para detectar gestos utilizando Inteligência Artificial no Arduino Nano BLE 33. Este projeto combina aprendizado de máquina e sensores para aplicações em dispositivos embarcados.  

---

### **🛠️ Tecnologias Utilizadas**  
- **TensorFlow Lite** para modelos TinyML  
- **Python** para pré-processamento e treinamento  
- **Arduino IDE** para implementação em C++  
- **Sensor IMU** do Nano BLE 33  

---

### **🌟 Funcionalidades**  
- Reconhecimento de quatro gestos diferentes.  
- Inferência em tempo real com baixo consumo de energia.  
- Aplicação prática para automação e controle por gestos.  

---

### **📌 Passo a Passo**  

#### 1️⃣ **Coleta de Dados**  
- Utilizar o sensor IMU para capturar dados do acelerômetro e do giroscópio (aX, aY, aZ, gX, gY, gZ).  
- Normalizar os dados com **MinMaxScaler** com base nos limites dos componentes.  

#### 2️⃣ **Treinamento do Modelo**  
- Treinar uma rede DNN (Dense Neural Network) no Python com entradas de dimensão `(400,)`.  
- Exportar o modelo em `.tflite`.  

#### 3️⃣ **Deploy no Arduino**  
- Converter o modelo para o formato `.h`.  
- Integrar o modelo ao código em C++ na IDE do Arduino.  

#### 4️⃣ **Teste e Validação**  
- Monitorar os resultados diretamente no Arduino.  

---

### **📊 Resultados Obtidos**  
- **Acurácia do modelo DNN Adam**: 97.5% nos testes.
- **Acurácia do modelo DNN SGD** : 97.5% nos testes.   

---

### **📂 Estrutura do Repositório**  
- **/Código_Captura**: script para coleta dos dados com o `.ino` de coleta;
- **/Código_Classificador**: script e modelos `.ino` e `.h` usados para a parte de testes;  
- **/Arduino_TinyML**: códigos e algoritmos em Python;  
- **/README**: parte explcitativa do projeto.  

---

### **🤝 Contribuições**  
Contribuições são bem-vindas!  

---

### **📧 Contato**  
- Autor: *João Victor Pelegrino*  
- Email: *pelegrino.jv@gmail.com* | *v.pelegrino@aluno.ifsp.edu.br* 

---

### **⭐ Dê um Star!**  
Se você gostou, deixe uma estrela no repositório!  

---  
