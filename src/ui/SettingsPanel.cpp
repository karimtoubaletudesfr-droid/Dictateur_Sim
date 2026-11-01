#include "dictatorsim/ui/SettingsPanel.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
namespace ds {
static std::string numeric(double x){std::ostringstream s;s<<std::fixed<<std::setprecision(2)<<x;return s.str();}
void SettingsPanel::open(const PlayerSession& session){draft_=session.simulation().world().config;active_=-1;audioDrag_=-1;feedback_.clear();confirmRestart_=false;}
bool SettingsPanel::commit(){
    if(active_==-1)return true;
    try{
        std::size_t end=0;double x=std::stod(input_,&end);
        if(end!=input_.size() || !std::isfinite(x))throw std::invalid_argument("Nombre invalide");
        if(active_==-2){if(x<1||x>1000000||std::floor(x)!=x)throw std::invalid_argument("Population : entier entre 1 et 1000000");draft_.population=static_cast<std::size_t>(x);}
        else {const auto& f=runtimeParameters()[active_];if(x<f.minimum||x>f.maximum)throw std::invalid_argument("Limites : "+numeric(f.minimum)+" a "+numeric(f.maximum));f.set(draft_,x);}
        active_=-1;feedback_.clear();return true;
    }catch(const std::exception& e){feedback_=e.what();return false;}
}
void SettingsPanel::event(const sf::Event& e){
    if(e.type==sf::Event::MouseButtonReleased || e.type==sf::Event::LostFocus)audioDrag_=-1;
    if(active_==-1)return;
    if(e.type==sf::Event::KeyPressed && e.key.control && e.key.code==sf::Keyboard::A)input_.clear();
    if(e.type==sf::Event::TextEntered){auto c=e.text.unicode;if(c==8){if(!input_.empty())input_.pop_back();}else if(input_.size()<16 && ((c>='0'&&c<='9')||c=='.'||c==','))input_+=c==','?'.':char(c);}
    if(e.type==sf::Event::KeyPressed && e.key.code==sf::Keyboard::Enter)commit();
}
SettingsChoice SettingsPanel::draw(sf::RenderTarget& t,UITheme& ui,PlayerSession& session,AudioManager& audio,sf::Vector2f mouse,bool click,bool held){
    ui.panel(t,"settings-shade",{0,0,1280,720},{5,10,18,235});
    ui.panel(t,"settings-body",{60,30,1160,660},ui.panelColor);
    ui.text(t,"settings-title","PARAMETRES DU PAYS",88,48,26,ui.ink);
    ui.text(t,"settings-help","Simulation en pause. Cliquez une valeur pour la saisir, puis Entree. Les boutons - / + ajustent la valeur.",88,85,13,ui.muted);
    const char* tabs[]={"Salaires","Evenements","Seuils","Economie","Population","Audio"};
    for(int i=0;i<6;++i)if(ui.button(t,"settings-tab"+std::to_string(i),tabs[i],{88+i*184.f,116,172,36},mouse,click,tab_==i)&&commit()){tab_=i;confirmRestart_=false;audioDrag_=-1;}
    if(!held && !click)audioDrag_=-1;
    if(tab_==5){
        ui.text(t,"audio-title","L'ambiance de votre presidence",112,182,24,ui.ink);
        ui.text(t,"audio-help","Cliquez sur une jauge ou faites glisser son curseur. 0 % coupe le son concerne.",112,222,14,ui.muted);
        for(int channel=0;channel<2;++channel){
            const float y=channel==0?300.f:450.f;
            std::string key="audio-"+std::to_string(channel);
            ui.roundPanel(t,{112,y-54,1060,133},{27,44,56},10);
            ui.text(t,key+"-label",channel==0?"MUSIQUE DE FOND":"SON DES BOUTONS",136,y-39,18,ui.ink);
            sf::FloatRect hit(220,y-14,840,36);
            if(click && hit.contains(mouse))audioDrag_=channel;
            if(audioDrag_==channel && (held || click)){
                float value=100*(mouse.x-220)/840;
                if(channel==0)audio.setMusicVolume(value);
                else {audio.setEffectsVolume(value);if(click)audio.playButton();}
            }
            float volume=channel==0?audio.musicVolume():audio.effectsVolume();
            ui.roundPanel(t,{220,y,840,10},{12,25,33},5);
            if(volume>0)ui.roundPanel(t,{220,y,840*volume/100,10},ui.accent,5);
            sf::CircleShape knob(11,28);knob.setOrigin(11,11);knob.setPosition(220+840*volume/100,y+5);
            knob.setFillColor(audioDrag_==channel?sf::Color(255,224,149):ui.ink);t.draw(knob);
            ui.text(t,key+"-zero","0",186,y-4,13,ui.muted);
            ui.text(t,key+"-value",std::to_string(static_cast<int>(volume))+" %",1083,y-5,16,ui.ink);
            bool ready=channel==0?audio.musicReady():audio.effectsReady();
            ui.text(t,key+"-description",!ready?"Fichier audio indisponible : verifiez assets/sounds.":channel==0?"background.mp3 / musique en boucle, y compris pendant la pause.":"button-press.mp3 / un son a chaque clic sur un bouton actif.",136,y+41,13,ready?ui.muted:ui.danger);
        }
        ui.text(t,"audio-session","Les deux volumes sont independants et conserves pendant cette session, meme apres une nouvelle partie.",112,550,13,ui.muted);
    }else if(tab_==4){
        ui.text(t,"population-title","Nombre de citoyens",92,184,24,ui.ink);
        ui.text(t,"population-desc","Population initiale du nouveau pays. Chaque citoyen est simule individuellement.\nUne population elevee demande davantage de calculs. Limites : 1 a 1 000 000.\nCe changement reconstruit le pays : jour, comptes et historique repartent de zero.",92,230,17,ui.muted);
        if(ui.button(t,"population-value",active_==-2?input_+"|":std::to_string(draft_.population),{92,330,300,46},mouse,click,active_==-2)&&commit()){active_=-2;input_=std::to_string(draft_.population);}
        if(ui.button(t,"population-new",confirmRestart_?"CONFIRMER LA REMISE A ZERO":"CREER UN PAYS AVEC CES REGLAGES",{92,410,550,48},mouse,click)&&commit()){
            if(confirmRestart_)return SettingsChoice::Restart;
            confirmRestart_=true;
        }
        ui.text(t,"population-confirm",confirmRestart_?"Cliquez a nouveau pour remplacer la partie actuelle. Retour ferme sans recreer le pays.":"Les autres onglets peuvent etre appliques a la partie actuelle sans remise a zero.",92,480,14,confirmRestart_?ui.danger:ui.muted);
    }else{
        int row=0;const auto& fields=runtimeParameters();
        for(std::size_t i=0;i<fields.size();++i){const auto& f=fields[i];if(f.category!=tabs[tab_])continue;
            float y=170+row++*53.f;std::string key="setting-"+std::to_string(i);
            ui.panel(t,key+"-row",{88,y,1104,49},row%2?sf::Color(27,42,56):sf::Color(23,36,49));
            ui.text(t,key+"-label",f.label,100,y+2,15,ui.ink);
            ui.text(t,key+"-desc",f.description,100,y+25,11,ui.muted);
            double value=f.get(draft_);
            if(ui.button(t,key+"-minus","-",{875,y+5,38,36},mouse,click)&&commit())f.set(draft_,std::max(f.minimum,f.get(draft_)-f.step));
            if(ui.button(t,key+"-value",active_==int(i)?input_+"|":numeric(value),{921,y+5,155,36},mouse,click,active_==int(i))&&commit()){active_=int(i);input_=numeric(f.get(draft_));}
            if(ui.button(t,key+"-plus","+",{1084,y+5,38,36},mouse,click)&&commit())f.set(draft_,std::min(f.maximum,f.get(draft_)+f.step));
            ui.text(t,key+"-unit",f.unit,1129,y+15,9,ui.muted);
        }
    }
    ui.text(t,"settings-feedback",tab_==5?"Volumes appliques immediatement.":feedback_.empty()?"Reglages conserves pour cette session. Les probabilites sont des bases, pas des frequences garanties.":feedback_,92,608,13,feedback_.empty()?ui.muted:ui.accent);
    if(ui.button(t,"settings-close","RETOUR",{88,642,170,34},mouse,click))return SettingsChoice::Close;
    if(tab_!=5 && ui.button(t,"settings-revert","ANNULER LES MODIFICATIONS",{280,642,320,34},mouse,click))open(session);
    if(tab_!=5 && ui.button(t,"settings-apply","APPLIQUER A LA PARTIE",{850,642,342,34},mouse,click,false,tab_!=4)&&commit()){
        try{session.applySettings(draft_);feedback_="Reglages appliques. Reprenez le temps pour observer leurs effets.";}catch(const std::exception& e){feedback_=e.what();}
    }
    return SettingsChoice::None;
}
}
