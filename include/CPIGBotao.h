#ifndef _CPIGBOTAO_
#define _CPIGBOTAO_

#include "CPIGComponente.h"

class CPIGBotao: public CPIGComponente{

protected:

    int tecla;

    virtual void ProcessaAtributos(CPIGAtributos atrib)override{
        CPIGComponente::ProcessaAtributos(atrib);
    }

    void SetFoco(bool valor){
        temFoco = valor;
    }

    void SetAcionado(bool valor){
        if (acionado&&!valor){
            Desloca(-margemEsq,margemBaixo);
            //SetDimensoes(alt-(margemCima+margemBaixo),larg-(margemEsq+margemDir));
        }else if (!acionado&&valor){
            Desloca(margemEsq,-margemBaixo);
            //SetDimensoes(alt+(margemCima+margemBaixo),larg+(margemEsq+margemDir));
        }
        acionado = valor;
        AjustaFrame();
    }

    void SetMouseOver(bool valor){
        mouseOver = valor;
        AjustaFrame();
    }

    virtual void AjustaFrame()=0;

    CPIGBotao(int idComponente, int alt, int larg, string nomeArq, int retiraFundo=1, int janela=0):
        CPIGComponente(idComponente,alt,larg,nomeArq,retiraFundo,janela){
            tecla = PIG_TECLA_ENTER;//sem tecla de atalho
            SetPosicaoPadraoLabel(PIG_COMPONENTE_CENTRO_CENTRO);
        }

    CPIGBotao(int idComponente, int alt, int larg, int janela=0):
        CPIGComponente(idComponente,alt,larg,janela){
            tecla = PIG_TECLA_ENTER;//sem tecla de atalho
            SetPosicaoPadraoLabel(PIG_COMPONENTE_CENTRO_CENTRO);
        }

    virtual ~CPIGBotao(){}

public:

    int TrataEventoMouse(PIGEvento evento){
        SDL_Point p = GetPosicaoMouse();

        //printf("mouse %d,%d\n",p.x,p.y);
        ChecaMouseOver(p);

        if (mouseOver){
            //printf("over\n");
            if (!habilitado) return PIG_SELECIONADO_DESABILITADO;
            if (!visivel) return PIG_SELECIONADO_INVISIVEL;
            if(evento.mouse.acao==PIG_MOUSE_PRESSIONADO && evento.mouse.botao == PIG_MOUSE_ESQUERDO) return OnAction();
            return PIG_SELECIONADO_MOUSEOVER;
        }

        return PIG_NAO_SELECIONADO;
    }

    virtual int TrataEventoTeclado(PIGEvento evento)=0;

    void DefineAtalho(int teclaAtalho){
        tecla = teclaAtalho;
    }

    void SetHabilitado(bool valor){
        habilitado = valor;
        AjustaFrame();
    }

    int Desenha()override{
        if (visivel==false) return 0;

        if (text)
            CPIGSprite::Desenha();
        else CPIGGerenciadorJanelas::GetJanela(idJanela)->DesenhaRetangulo((int)pos.x,(int)pos.y,alt,larg,coresBasicas[0]);

        DesenhaLabel();

        EscreveHint();

        return 1;
    }

};

typedef CPIGBotao *PIGBotao;
#endif // _CPIGBOTAO_
