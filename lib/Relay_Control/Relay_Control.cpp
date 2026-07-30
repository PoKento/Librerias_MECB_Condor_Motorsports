#include "Relay_Control.h"
#include "Arduino.h"


/**
 * Inicializa el control de los relés. Los relés inician automaticamente apagados (Útil para el killswitch).
 * @param SolarPin  Pin conectado al relé del contactor que desconecta los paneles solares.
 * @param BattPin   Pin conectado al relé del contactor que desconecta la batería principal.
 */
Relay_Control::Relay_Control(int SolarPin, int BattPin){
    _SolarPin = SolarPin;
    _BattPin = BattPin;

    pinMode(_SolarPin, OUTPUT);
    pinMode(_BattPin, OUTPUT);

    digitalWrite(_SolarPin, LOW);
    digitalWrite(_BattPin, LOW);
}

/**
 * Apaga los relés de los paneles solares y batería principal.
 */
void Relay_Control::off(){
    digitalWrite(_SolarPin, LOW);
    digitalWrite(_BattPin, LOW);
}

/**
 * Apaga los relés de los paneles solares y batería principal.
 */
void Relay_Control::on(){
    digitalWrite(_SolarPin, HIGH);
    digitalWrite(_BattPin, HIGH);
}
