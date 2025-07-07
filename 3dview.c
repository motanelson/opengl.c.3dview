#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define BLOCK_SIZE 0.1f
#define ROTATE_INTERVAL 900 // 2 segundos
//sudo apt install freeglut3-dev
//gcc voxel_viewer.c -o voxel_viewer -lGL -lGLU -lglut -lm

int sizeX, sizeY, sizeZ,sizeXX;
uint8_t ***voxels = NULL;
float angle = 0.0f;

GLfloat vga_colors[16][3] = {
    {0.0, 0.0, 0.0}, {0.0, 0.0, 0.667}, {0.0, 0.667, 0.0}, {0.0, 0.667, 0.667},
    {0.667, 0.0, 0.0}, {0.667, 0.0, 0.667}, {0.667, 0.333, 0.0}, {0.667, 0.667, 0.667},
    {0.333, 0.333, 0.333}, {0.333, 0.333, 1.0}, {0.333, 1.0, 0.333}, {0.333, 1.0, 1.0},
    {1.0, 0.333, 0.333}, {1.0, 0.333, 1.0}, {1.0, 1.0, 0.333}, {1.0, 1.0, 1.0}
};

int load_voxels(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;

    char header[2];
    fread(header, 1, 2, f);
    if (header[0] != '3' || header[1] != 'D') {
        fclose(f);
        return 0;
    }

    sizeX = fgetc(f);
    sizeXX = fgetc(f);
    sizeY = fgetc(f);
    sizeZ = fgetc(f);

    voxels = malloc(sizeZ * sizeof(uint8_t **));
    for (int z = 0; z < sizeZ; z++) {
        voxels[z] = malloc(sizeY * sizeof(uint8_t *));
        for (int y = 0; y < sizeY; y++) {
            voxels[z][sizeY-1-y] = malloc(sizeX * sizeof(uint8_t));
            fread(voxels[z][sizeY-1-y], 1, sizeX, f);
        }
    }

    fclose(f);
    return 1;
}

void draw_cube(float x, float y, float z, int color) {
    glColor3fv(vga_colors[color % 16]);
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidCube(BLOCK_SIZE);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(2.0, 2.0, 4.0, 0, 0, 0, 0, 1, 0);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    float offsetX = -(sizeX * BLOCK_SIZE) / 2.0f;
    float offsetY = -(sizeY * BLOCK_SIZE) / 2.0f;
    float offsetZ = -(sizeZ * BLOCK_SIZE) / 2.0f;

    for (int z = 0; z < sizeZ; z++) {
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                uint8_t color = voxels[z][y][x];
                if (color == 0) continue;
                draw_cube(offsetX + x * BLOCK_SIZE, offsetY + y * BLOCK_SIZE, offsetZ + z * BLOCK_SIZE, color);
            }
        }
    }

    glutSwapBuffers();
}

void timer(int value) {
    angle += 15.0f;
    if (angle >= 360.0f) angle -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(ROTATE_INTERVAL, timer, 0);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Fundo amarelo
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    if (argc < 2 || !load_voxels(argv[1])) {
        printf("Erro: use %s ficheiro.3d\n", argv[0]);
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Visualizador .3D tipo Minecraft");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(ROTATE_INTERVAL, timer, 0);
    glutMainLoop();

    return 0;
}
