#ifndef _CPIGJanela_
#define _CPIGJanela_

#include "CPIGCamera.h"
#include "CPIGOffscreenRenderer.h"

class CPIGJanela{

private:

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *textFundo;
PIG_Cor corFundo;
int altura,largura,px,py;
int id;
int fechada,modo;
float opacidade;
std::string titulo;
PigCamera camera;

public:

CPIGJanela(std::string tituloJanela,int idJanela,int altTela,int largTela){
    id = idJanela;
    titulo = tituloJanela;
    altura = altTela;
    largura = largTela;
    window = SDL_CreateWindow( tituloJanela.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, largura, altura, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL );
    if( window == NULL ){
        printf( "Janela %d nao pode ser criada! Erro da SDL: %s\n",id, SDL_GetError() );
    }else{
        //handler = SDL_GetWindowID(window);
        //printf("Meu id %d meu handler %d\n",idJanela,handler);
        SDL_GetWindowPosition(window,&px,&py);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
        corFundo = PRETO;
        SDL_SetRenderDrawColor( renderer, corFundo.r, corFundo.g, corFundo.b, corFundo.a );
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        fechada = 0;
        modo = PIG_JANELA_NORMAL;
        camera = new CPigCamera(altTela,largTela);
    }

    opacidade = 1.0f;
    textFundo = NULL;
}

~CPIGJanela(){
    SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    delete camera;
}

SDL_Window *GetWindow(){
    return window;
}

SDL_Renderer *GetRenderer(){
    return renderer;
}

void Fecha(){
    if (window==NULL) return;
    SDL_DestroyWindow(window);
    window = NULL;
    fechada = 1;
}

int GetFechada(){
    return fechada;
}

void Esconde(){
    SDL_HideWindow(window);
}

void Exibe(){
    SDL_ShowWindow(window);
}

void GanhaFoco(){
    SDL_RaiseWindow(window);
}

void MoveCamera(int nx, int ny){
    camera->Move(nx,ny);
}

void DeslocaCamera(int dx, int dy){
    camera->Desloca(dx,dy);
}

void SetZoom(float zoom){
    float novoZoom = camera->SetZoom(zoom);
    SDL_RenderSetLogicalSize(renderer,novoZoom*largura,novoZoom*altura);
}

void ConverteCoordenadaScreenWorld(int screenX, int screenY, int &worldX, int &worldY){
    camera->ConverteCoordenadaScreenWorld(screenX, screenY, worldX, worldY);
}

void ConverteCoordenadaWorldScreen(int worldX, int worldY, int &screenX, int &screenY){
    camera->ConverteCoordenadaWorldScreen( worldX, worldY, screenX, screenY);
}

void DefineFundo(std::string nomeArquivo){
    SDL_Surface* bitmap = IMG_Load(nomeArquivo.c_str());
    if (textFundo)
        SDL_DestroyTexture(textFundo);
    textFundo = SDL_CreateTextureFromSurface(renderer,bitmap);
    SDL_FreeSurface(bitmap);
}

void SaveScreenshot(std::string nomeArquivo, bool BMP) {
    if (window==NULL) return;
    SDL_Surface* saveSurface = NULL;
    SDL_Surface* infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(window);
    if (infoSurface != NULL) {
        unsigned char * pixels = new unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
        if (pixels != 0) {
            if (SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) == 0) {
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface != NULL) {
                    if (BMP)
                        SDL_SaveBMP(saveSurface, nomeArquivo.c_str());
                    else IMG_SavePNG(saveSurface, nomeArquivo.c_str());
                    SDL_FreeSurface(saveSurface);
                }
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
    }
}

void IniciaDesenho(){
    if (textFundo==NULL) {
        SDL_SetRenderDrawColor(renderer,corFundo.r,corFundo.g,corFundo.b,corFundo.a);
        SDL_RenderClear( renderer );
    }
    else SDL_RenderCopy(renderer,textFundo,NULL,NULL);
}

void EncerraDesenho(){
    SDL_RenderPresent(renderer);

}

/*void SetLogicalSize(int novaaltura, int novalargura){
    SDL_RenderSetLogicalSize(renderer,novalargura,novaaltura);
    //printf("nova %d desloca %d\n",novaaltura,altura-novaaltura);
    camera->Move(camera->GetX(),-altura+novaaltura);
}*/

int GetAltura(){
    return altura;
}

int GetLargura(){
    return largura;
}

std::string GetTitulo(){
    return titulo;
}

void SetTitulo(std::string novoTitulo){
    if (window==NULL) return;
    titulo = novoTitulo;
    SDL_SetWindowTitle(window,titulo.c_str());
}

PIG_Cor GetCorFundo(){
    return corFundo;
}

void SetCorFundo(PIG_Cor cor){
    corFundo = cor;
}

float GetOpacidade(){
    return opacidade;
}

void SetOpacidade(float valor){
    opacidade = valor;
    SDL_SetWindowOpacity(window,opacidade);
}

void SetPosicao(int x,int y){
    if (window==NULL) return;
    SDL_SetWindowPosition(window,x,y);
}

void GetPosicao(int &x,int &y){
    x = px;
    y = py;
}

void SetBorda(int valor){
    if (window==NULL) return;
    SDL_SetWindowBordered(window,SDL_bool(valor));
}

void SetModo(int valor){
    if (window==NULL) return;
    modo = valor;
    SDL_SetWindowFullscreen(window,modo);
    SDL_GetWindowSize(window,&largura,&altura);
}

int GetModo(){
    return modo;
}

/*int SetTamanho(int alt, int larg){
    altura = alt;
    largura = larg;
    SDL_SetWindowSize(window,larg,alt);
    //SDL_GetWindowSize(window,&largura,&altura);
    camera->AjustaTela(altura,largura);
    SetZoom(1);
}*/

    void DesenhaRetangulo(int x, int y, int alturaRet, int larguraRet, PIG_Cor cor){
        SDL_Rect rect;
        rect.x = x;
        rect.y = altura-(y+alturaRet);
        rect.h = alturaRet;
        rect.w = larguraRet;

        camera->ConverteCoordenadaWorldScreen(rect.x,rect.y,rect.x,rect.y);

        SDL_SetRenderDrawColor(renderer, cor.r,cor.g,cor.b,cor.a);
        SDL_RenderFillRect(renderer,&rect);
    }

    void DesenhaRetanguloVazado(int x, int y, int alturaRet, int larguraRet, PIG_Cor cor){
        SDL_Rect rect;
        rect.x = x;
        rect.y = altura-(y+alturaRet);
        rect.h = alturaRet;
        rect.w = larguraRet;

        camera->ConverteCoordenadaWorldScreen(rect.x,rect.y,rect.x,rect.y);

        SDL_SetRenderDrawColor(renderer, cor.r,cor.g,cor.b,cor.a);
        SDL_RenderDrawRect(renderer,&rect);
    }

    void DesenhaLinhaSimples(int x1,int y1,int x2,int y2,PIG_Cor cor){
        SDL_SetRenderDrawColor(renderer,cor.r,cor.g,cor.b,255);
        int camX1,camY1,camX2,camY2;
        camera->ConverteCoordenadaWorldScreen(x1,altura-y1-1,camX1,camY1);
        camera->ConverteCoordenadaWorldScreen(x2,altura-y2-1,camX2,camY2);
        SDL_RenderDrawLine(renderer,camX1,camY1,camX2,camY2);
        //SDL_RenderDrawLine(renderer,x1-camera->GetX(),altura-y1-1+camera->GetY(),x2-camera->GetX(),altura-y2-1+camera->GetY());
    }

    void DesenhaLinhasDisjuntas(int *x,int *y,int qtd,PIG_Cor cor){
        SDL_SetRenderDrawColor(renderer,cor.r,cor.g,cor.b,255);
        int camX1,camY1,camX2,camY2;
        for (int k=0;k<qtd*2;k+=2){
            camera->ConverteCoordenadaWorldScreen(x[k],altura-y[k],camX1,camY1);
            camera->ConverteCoordenadaWorldScreen(x[k+1],altura-y[k+1],camX2,camY2);
            SDL_RenderDrawLine(renderer,camX1,camY1,camX2,camY2);
            //SDL_RenderDrawLine(renderer,x[k]-camera->GetX(),altura-y[k]+camera->GetY(),x[k+1]-camera->GetX(),altura-y[k+1]+camera->GetY());
        }
    }

    void DesenhaLinhasSequencia(int *x,int *y,int qtd,PIG_Cor cor){
        SDL_SetRenderDrawColor(renderer,cor.r,cor.g,cor.b,255);
        int camX1,camY1,camX2,camY2;
        for (int k=0;k<qtd-1;k++){
            camera->ConverteCoordenadaWorldScreen(x[k],altura-y[k],camX1,camY1);
            camera->ConverteCoordenadaWorldScreen(x[k+1],altura-y[k+1],camX2,camY2);
            SDL_RenderDrawLine(renderer,camX1,camY1,camX2,camY2);
            //SDL_RenderDrawLine(renderer,x[k]-camera->GetX(),altura-y[k]+camera->GetY(),x[k+1]-camera->GetX(),altura-y[k+1]+camera->GetY());
        }
    }

    void DesenhaPoligono(int px[],int py[],int lados,PIG_Cor cor){
        int minX=99999,maxX=-1,minY=99999,maxY=-1;
        int cx=0,cy=0;

        //calcula o bounding-box do poligono
        for (int i=0;i<lados;i++){
            if (px[i]<minX) minX = px[i];
            if (py[i]<minY) minY = py[i];
            if (px[i]>maxX) maxX = px[i];
            if (py[i]>maxY) maxY = py[i];
            cx += px[i]; //centro do poligono
            cy += py[i]; //centro do poligono
        }
        cx /= lados; //centro do poligono
        cy /= lados; //centro do poligono

        int alt = maxY-minY+1;  //altura absoluta do poligono
        int larg = maxX-minX+1; //altura absoluta do poligono

        PIGOffscreenRenderer off = new CPIGOffscreenRenderer(alt,larg); //ajustado extamente com a altura e largura

        if (PIGCoresIguais(cor,PRETO)){
            off->PintarFundo(BRANCO);
        }else off->PintarFundo(PRETO);

        for (int i=0;i<lados;i++)
            off->DesenharLinha(px[i]-minX,py[i]-minY,px[(i+1)%lados]-minX,py[(i+1)%lados]-minY,cor);

        off->PintarArea(cx-minX,cy-minY,cor,NULL);

        SDL_Surface *surf = off->GetSurface();
        if (PIGCoresIguais(cor,PRETO)){
            SDL_SetColorKey( surf, SDL_TRUE, SDL_MapRGBA(surf->format, 255, 255, 255, 255) );
        }else SDL_SetColorKey( surf, SDL_TRUE, SDL_MapRGBA(surf->format, 0, 0, 0, 255) );

        //SDL_Renderer *renderer = CGerenciadorJanelas::GetJanela(idJanela)->GetRenderer();
        SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect r;
        r.h = alt;
        r.w = larg;
        r.x = minX;
        r.y = altura-minY-alt;
        camera->ConverteCoordenadaWorldScreen(r.x,r.y,r.x,r.y);
        //printf("%d,%d %d,%d\n",r.x,r.y,r.h,r.w);
        SDL_RenderCopy(renderer,text,NULL,&r);
        SDL_DestroyTexture(text);
        delete off;
    }

    PIG_Cor GetPixel(int x,int y) {
        if (x<0 || x>=largura  ||y<0 ||y>=altura) return PRETO;
        PIG_Cor resp;
        SDL_Surface* infoSurface = SDL_GetWindowSurface(window);
        if (infoSurface != NULL) {
            unsigned char * pixels = new unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
            if (pixels != 0) {
                if (SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) == 0) {
                    Uint32 *p = (Uint32*) pixels;
                    p += (x+(altura-y)*largura);
                    SDL_GetRGBA(*p,(const SDL_PixelFormat*) &infoSurface->format->format,&resp.r,&resp.g,&resp.b,&resp.a);
                }
                delete[] pixels;
            }
            SDL_FreeSurface(infoSurface);
        }
        return resp;
    }

};

typedef CPIGJanela *PIGJanela;
#endif // _CPIGJanela_