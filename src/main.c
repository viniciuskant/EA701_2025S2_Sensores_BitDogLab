#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>
#include "tcs34725.h"

#define BUTTON_PIN 5 // botão A
#define NUM_CORES (sizeof(referencias) / sizeof(referencias[0]))
#define NUM_CORES_HSL (sizeof(cores_hsl) / sizeof(cores_hsl[0]))


int cor_atual_calibracao = 0;
bool calibracao_concluida = false;


// struct para conter o intervalo de cores rgb para cada cor testada
typedef struct {
    uint16_t r_min, r_max;
    uint16_t g_min, g_max;
    uint16_t b_min, b_max;
    const char* nome;
} cor_referencia_t;

cor_referencia_t referencias[] = {
    {0, 0, 0, 0, 0, 0, "Vermelho"},
    {0, 0, 0, 0, 0, 0, "Verde"},
    {0, 0, 0, 0, 0, 0, "Azul"},
    {0, 0, 0, 0, 0, 0, "Amarelo"},
    {0, 0, 0, 0, 0, 0, "Magenta"},
    {0, 0, 0, 0, 0, 0, "Ciano"},
    {0, 0, 0, 0, 0, 0, "Branco"},
    {0, 0, 0, 0, 0, 0, "Preto"}
};


// Faixas de matiz - HSL
typedef struct {
    const char* nome;
    float hue_min;
    float hue_max;
} cor_hsl_ref_t;

cor_hsl_ref_t cores_hsl[] = {
    {"Vermelho", 350, 360},
    {"Vermelho", 0, 10},
    {"Laranja", 11, 40},
    {"Amarelo", 41, 70},
    {"Verde", 71, 170},
    {"Ciano", 171, 200},
    {"Azul", 201, 260},
    {"Magenta", 261, 315},
    {"Rosa", 316, 349}
};


// struct que contém as coordenadas da cor em hsl
typedef struct {
    float h, s, l;
} hsl_t;

hsl_t rgb_to_hsl(uint16_t r, uint16_t g, uint16_t b) {
    float fr = r / 65535.0f;
    float fg = g / 65535.0f;
    float fb = b / 65535.0f;

    float max = fmaxf(fmaxf(fr, fg), fb);
    float min = fminf(fminf(fr, fg), fb);

    float delta = max - min;

    hsl_t hsl;
    hsl.l = (max + min) / 2.0f;

    if (delta == 0.0f) {
        hsl.h = 0;
        hsl.s = 0;
        return hsl;
    }

    hsl.s = (hsl.l < 0.5f) ? (delta / (max + min)) : (delta / (2.0f - max - min));

    if (max == fr)
        hsl.h = 60.0f * fmodf(((fg - fb) / delta), 6.0f);
    else if (max == fg)
        hsl.h = 60.0f * (((fb - fr) / delta) + 2.0f);
    else
        hsl.h = 60.0f * (((fr - fg) / delta) + 4.0f);

    if (hsl.h < 0) hsl.h += 360.0f;

    return hsl;
}


const char* detectar_cor_calibrada(void) {
    color_data_t data;
    tcs34725_read(&data);

    if (data.r == 0 && data.g == 0 && data.b == 0)
        return "Sem luz";

    // primeira tentativa: comparação com intervalos calibrados (RGB)
    for (int i = 0; i < NUM_CORES; i++) {
        if (data.r >= referencias[i].r_min && data.r <= referencias[i].r_max &&
            data.g >= referencias[i].g_min && data.g <= referencias[i].g_max &&
            data.b >= referencias[i].b_min && data.b <= referencias[i].b_max) {
            return referencias[i].nome;
        }
    }

    // segunda tentativa: interpolação via HSL (cor não calibrada)
    hsl_t hsl = rgb_to_hsl(data.r, data.g, data.b);

    for (int i = 0; i < NUM_CORES_HSL; i++) {
        if (hsl.h >= cores_hsl[i].hue_min && hsl.h <= cores_hsl[i].hue_max) {
            return cores_hsl[i].nome;
        }
    }

    return "Cor interpolada desconhecida";
}


void capturar_cor_referencia(int indice_cor) {
    printf("Capturando referencia para %s...\n", referencias[indice_cor].nome);
    printf("Posicione a cor %s sobre o sensor e pressione o botao...\n", referencias[indice_cor].nome);
    
    while (gpio_get(BUTTON_PIN) == 1) {
        sleep_ms(10);
    }
    
    sleep_ms(200);
    
    color_data_t data;
    uint32_t soma_r = 0, soma_g = 0, soma_b = 0;
    const int num_leituras = 10;
    
    for (int i = 0; i < num_leituras; i++) {
        tcs34725_read(&data);
        soma_r += data.r;
        soma_g += data.g;
        soma_b += data.b;
        sleep_ms(50);
    }
    
    // Calcula médias
    uint16_t media_r = soma_r / num_leituras;
    uint16_t media_g = soma_g / num_leituras;
    uint16_t media_b = soma_b / num_leituras;
    
    // Define os intervalos com uma margem de 30%
    referencias[indice_cor].r_min = media_r * 0.85;
    referencias[indice_cor].r_max = media_r * 1.15;
    referencias[indice_cor].g_min = media_g * 0.85;
    referencias[indice_cor].g_max = media_g * 1.15;
    referencias[indice_cor].b_min = media_b * 0.85;
    referencias[indice_cor].b_max = media_b * 1.15;
    
    printf("Referencia para %s capturada!\n", referencias[indice_cor].nome);
    printf("R: %d (%d-%d), G: %d (%d-%d), B: %d (%d-%d)\n", 
           media_r, referencias[indice_cor].r_min, referencias[indice_cor].r_max,
           media_g, referencias[indice_cor].g_min, referencias[indice_cor].g_max,
           media_b, referencias[indice_cor].b_min, referencias[indice_cor].b_max);
    
    while (gpio_get(BUTTON_PIN) == 0) {
        sleep_ms(10);
    }
    
    sleep_ms(200);
}

void calibrar_cores(void) {
    printf("=== INICIANDO CALIBRACAO ===\n");
    printf("Voce ira calibrar %d cores.\n", NUM_CORES);
    
    for (int i = 0; i < NUM_CORES; i++) {
        capturar_cor_referencia(i);
        
        if (i < NUM_CORES - 1) {
            printf("Próxima cor: %s\n", referencias[i + 1].nome);
        }
    }
    
    calibracao_concluida = true;
    printf("=== CALIBRACAO CONCLUIDA ===\n");
}

void mostrar_referencias(void) {
    printf("\n=== REFERENCIAS CALIBRADAS ===\n");
    for (int i = 0; i < NUM_CORES; i++) {
        printf("%s: R(%d-%d) G(%d-%d) B(%d-%d)\n",
               referencias[i].nome,
               referencias[i].r_min, referencias[i].r_max,
               referencias[i].g_min, referencias[i].g_max,
               referencias[i].b_min, referencias[i].b_max);
    }
    printf("\n");
}

int main() {
    stdio_init_all();
    
    sleep_ms(2000);
    printf("Iniciando sistema de detecção de cores...\n");

    tcs34725_init();
    printf("Sensor TCS34725 inicializado!\n");

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    calibrar_cores();
    
    mostrar_referencias();
    
    printf("Sistema pronto para deteccao de cores!\n");

    while (true) {
        const char* cor = detectar_cor_calibrada();
        printf("Cor detectada: %s\n", cor);
        sleep_ms(500);
    }

    return 0;
}