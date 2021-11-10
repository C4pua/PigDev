#ifndef _CPIGGAUGEBAR_
#define _CPIGGAUGEBAR_

#include "CPIGGauge.h"

class CPIGGaugeBar: public CPIGGauge{

private:

    inline void IniciaCoresBasicas(){
        coresBasicas[0] = BRANCO;
        coresBasicas[1] = VERMELHO;
        coresBasicas[2] = VERMELHO;
    }

    static CPIGGaugeBar LeParametros(int idComponente, CPIGAtributos atrib){
        CPIGGaugeBar *resp;

        if (atrib.GetString("nomeArq","")!=""){
            resp = new CPIGGaugeBar(idComponente,atrib.GetInt("px",0),atrib.GetInt("py",0),atrib.GetInt("altura",0),atrib.GetInt("largura",0),
                          atrib.GetString("nomeArq",""),atrib.GetString("nomeArqMarcador",""),
                          atrib.GetInt("retiraFundo",1),atrib.GetInt("retiraFundoMarcador",1),atrib.GetInt("janela",0));
        }else{
            resp = new CPIGGaugeBar(idComponente,atrib.GetInt("px",0),atrib.GetInt("py",0),atrib.GetInt("altura",0),atrib.GetInt("largura",0),
                          atrib.GetInt("janela",0));
        }

        resp->ProcessaAtributosGerais(atrib);

        return *resp;
    }

    void DesenhaBarraCor(){
        PIGCor cor = PIGMixCor(coresBasicas[1],coresBasicas[2],porcentagemConcluida);
        double perc = porcentagemConcluida;
        int largBarra = larg-margemDir-margemEsq;
        int altBarra = alt-margemBaixo-margemCima;

        switch(orientacaoCrescimento){
        case PIG_GAUGE_ESQ_DIR:
            CPIGGerenciadorJanelas::GetJanela(idJanela)->DesenhaRetangulo(((int)pos.x)+margemEsq,((int)pos.y)+margemBaixo,altBarra,((int)largBarra*perc),cor);
            break;
        case PIG_GAUGE_DIR_ESQ:
            CPIGGerenciadorJanelas::GetJanela(idJanela)->DesenhaRetangulo(pos.x+larg-margemDir-largBarra*perc,pos.y+margemBaixo,altBarra,largBarra*perc,cor);
            break;
        case PIG_GAUGE_BAIXO_CIMA:
            CPIGGerenciadorJanelas::GetJanela(idJanela)->DesenhaRetangulo(pos.x+margemEsq,pos.y+margemBaixo,altBarra*perc,largBarra,cor);
            break;
        case PIG_GAUGE_CIMA_BAIXO:
            CPIGGerenciadorJanelas::GetJanela(idJanela)->DesenhaRetangulo(pos.x+margemEsq,pos.y+alt-margemCima-altBarra*perc,altBarra*perc,largBarra,cor);
            break;
        }
    }

    virtual void AtualizaMarcador()override{
        OnAction();
        if (marcador==NULL) return;

        int altBarra=0,largBarra=0;
        int altBarraOrig=0,largBarraOrig=0;

        marcador->GetDimensoesOriginais(altBarraOrig,largBarraOrig);
        PIGPonto2D posBarra={0,0};

        double perc = porcentagemConcluida;
        SDL_Rect r={0,0,0,0};

        //printf(" porc %f\n",perc);

        switch(orientacaoCrescimento){
        case PIG_GAUGE_ESQ_DIR:
            altBarra = alt-margemCima-margemBaixo;
            largBarra = (larg-margemDir-margemEsq)*perc;
            posBarra = {pos.x+margemEsq,pos.y+margemBaixo};
            r.x = r.y = 0;
            r.w = largBarra*perc;
            r.h = altBarra;
            break;
        case PIG_GAUGE_DIR_ESQ:
            altBarra = alt-margemCima-margemBaixo;
            largBarra = (larg-margemDir-margemEsq)*perc;
            posBarra = {pos.x+larg-margemDir-largBarra,pos.y+margemBaixo};
            r.x = largBarraOrig*(1-perc);
            r.y = 0;
            r.w = largBarraOrig*perc;
            r.h = altBarra;
            break;
        case PIG_GAUGE_BAIXO_CIMA:
            altBarra = (alt-margemCima-margemBaixo)*perc;
            largBarra = larg-margemDir-margemEsq;
            posBarra = {pos.x+margemEsq,pos.y+margemBaixo};
            r.x = 0;
            r.y = altBarraOrig*(1-perc);
            r.w = largBarraOrig;
            r.h = altBarraOrig*perc;
            break;
        case PIG_GAUGE_CIMA_BAIXO:
            altBarra = (alt-margemCima-margemBaixo)*perc;
            largBarra = larg-margemDir-margemEsq;
            posBarra = {pos.x+margemEsq,pos.y+alt-margemCima-altBarra};
            r.x = r.y = 0;
            r.w = largBarraOrig;
            r.h = altBarraOrig*perc;
            break;
        }

        //printf("x %d y %d alt %d larg %d %d,%d,%d,%d\n",(int)posBarra.x,(int)posBarra.y,altBarra,largBarra,margemEsq,margemDir,margemCima,margemBaixo);
        marcador->DefineFrame(1,r);
        marcador->SetDimensoes(altBarra,largBarra);
        marcador->MudaFrameAtual(1);
        marcador->Move((int)posBarra.x,(int)posBarra.y);
    }

public:

    CPIGGaugeBar(int idComponente, int px, int py, int altura, int largura, string imgMoldura, string imgMarcador, int retiraFundoTrilha=1, int retiraFundoMarcador=1, int janela=0):
        CPIGGauge(idComponente,px,py,altura,largura,imgMoldura,altura,largura,imgMarcador,retiraFundoTrilha,retiraFundoMarcador,janela){
        IniciaCoresBasicas();
        if (imgMarcador!=""){
            marcador = new CPIGSprite(-1,imgMarcador,retiraFundoMarcador,NULL,janela);
            AtualizaMarcador();
        }
    }

    CPIGGaugeBar(int idComponente, int px, int py, int altura, int largura, int janela=0):
        CPIGGauge(idComponente,px,py,altura,largura,janela){
        IniciaCoresBasicas();
    }

    virtual ~CPIGGaugeBar(){
        if (marcador) delete marcador;
    }

    CPIGGaugeBar(int idComponente, CPIGAtributos atrib):CPIGGaugeBar(LeParametros(idComponente,atrib)){}

    int Desenha(){

        //moldura
        if (text)
            CPIGSprite::Desenha();
        else CPIGGerenciadorJanelas::GetJanela(idJanela)->DesenhaRetangulo((int)pos.x,(int)pos.y,alt,larg,coresBasicas[0]);

        //Desenha Barra
        if (marcador)
            marcador->Desenha();
        else{
            DesenhaBarraCor();
        }

        DesenhaLabel();

        EscreveHint();

        return 1;
    }

    void SetCorFinal(PIGCor cor){
        coresBasicas[2] = cor;
    }

    void SetCorInicial(PIGCor cor){
        coresBasicas[1] = cor;
    }

    void SetCorTrilha(PIGCor cor){
        coresBasicas[0] = cor;
    }

    int TrataEventoMouse(PIGEvento evento){
        return 0;
    }

    int TrataEventoTeclado(PIGEvento evento){
        return 0;
    }

    virtual void SetMargens(int mEsq,int mDir,int mCima,int mBaixo)override{
        CPIGComponente::SetMargens(mEsq,mDir,mCima,mBaixo);
        AtualizaMarcador();
    }

};
typedef CPIGGaugeBar *PIGGaugeBar;
#endif // _CPIGGAUGE_
