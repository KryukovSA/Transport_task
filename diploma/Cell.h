#pragma once

enum statusInHalfChain { positive, negative, neutral};
enum cellStatus {basic, free_};

class Cell {
    double tarif;
    statusInHalfChain signInHalfChain;
    double defferncTarifAndPotincials;
    cellStatus status;
    int cargoVolueme; //also see for double type

public:
    Cell() {
        tarif = 0;
        signInHalfChain = neutral;
        defferncTarifAndPotincials = 10000; //(tarif - (u+v))
        status = free_;
        cargoVolueme = 0;
    }

    void set_signInHalfChain(statusInHalfChain signInHalfChain_) { signInHalfChain = signInHalfChain_; }
    void set_tarif(double tarif_) { tarif = tarif_; }
    void set_defferncTarifAndPotincials(double defferncTarifAndPotincials_) { defferncTarifAndPotincials = defferncTarifAndPotincials_; }
    void set_status(cellStatus status_) { status = status_; }
    void set_cargoVolueme(int cargoVolueme_) { cargoVolueme = cargoVolueme_; };


    double get_tarif() { return tarif; }
    statusInHalfChain get_signInHalfChain() { return signInHalfChain; }
    double get_defferncTarifAndPotincials() { return defferncTarifAndPotincials; }
    cellStatus get_status() { return status; }
    int get_cargoVolueme() { return cargoVolueme; }


};



