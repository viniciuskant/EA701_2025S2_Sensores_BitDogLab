# Relatório Técnico da Dupla

## 1. Escopo e Objetivos
Descreva de forma clara o propósito do experimento ou desenvolvimento realizado, delimitando o escopo do trabalho e os objetivos técnicos a serem atingidos.  
Inclua também os critérios de sucesso utilizados para validar o funcionamento do sistema.  

**Exemplo:**  
> Implementar a leitura do sensor AHT20 via interface I²C e exibir em tempo real a temperatura e a umidade relativa no display OLED da BitDogLab, garantindo estabilidade de leitura e compatibilidade com a biblioteca MicroPython oficial.

---

## 2. Metodologia e Implementação
Apresente os procedimentos adotados, incluindo a arquitetura do sistema, estratégias de programação, bibliotecas utilizadas e justificativas de projeto.  
Se aplicável, descreva o esquema de ligação elétrica, o protocolo de comunicação, as rotinas de aquisição e processamento de dados e o fluxo lógico do programa.  

**Sugestão de tópicos a abordar:**
- Diagrama de blocos e descrição funcional dos módulos;  
- Linguagem e ambiente de desenvolvimento utilizados;  
- Configurações específicas da BitDogLab e periféricos;  
- Estratégias de depuração e calibração.

---

## 3. Resultados e Análise
Apresente os principais resultados obtidos, acompanhados de gráficos, tabelas ou imagens que sustentem a análise.  
Discuta o comportamento observado, eventuais desvios em relação ao esperado e hipóteses para as causas identificadas.  

**Inclua, sempre que possível:**
- Valores medidos e respectivos limites de erro;  
- Comparação entre medições experimentais e dados de referência do datasheet;  
- Registros fotográficos do setup de teste;  
- Logs ou capturas de tela relevantes.

---

## 4. Dificuldades e Soluções
Relate os principais desafios técnicos enfrentados e as soluções implementadas.  
Explique como eventuais limitações foram contornadas ou mitigadas, de modo a registrar aprendizados úteis para reproduções futuras.

**Exemplos:**
- Ajuste de timing no barramento I²C;  
- Necessidade de adaptação da biblioteca para MicroPython;  
- Filtragem de ruído em leitura analógica.

---

## 5. Conclusões e Trabalhos Futuros
Resuma as conclusões técnicas alcançadas e a avaliação crítica dos resultados.  
Indique aprimoramentos possíveis e oportunidades de extensão do trabalho, incluindo aplicações derivadas ou integração com outros módulos.

**Exemplos:**
- Otimizar a estabilidade do sinal por meio de filtragem digital;  
- Adaptar o código para o ambiente C/C++ (Pico SDK);  
- Integrar múltiplos sensores e consolidar dados via comunicação serial.

---

## 6. Referências
Liste as fontes técnicas e documentações consultadas, como datasheets, manuais de aplicação, artigos ou links de bibliotecas utilizadas.  
O formato de citação é livre, desde que contenha autor, título e origem.

