extern int mode;

void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void setCamera(void);
void resetCamera(void);
void renderBitmapString(float x,float y,void *font,std::string string);