#pragma once
enum statusInHalfChain { positive, negative, neutral};
class Cell {
    double tarif;
    statusInHalfChain signInHalfChain;

public:
    Cell() {
        tarif = 0;
        signInHalfChain = neutral;
    }
    void set_signInHalfChain(statusInHalfChain signInHalfChain_) { signInHalfChain = signInHalfChain_; }
    void set_tarif(double tarif_) { tarif = tarif_; }

    double get_tarif() { return tarif; }
    statusInHalfChain get_signInHalfChain() { return signInHalfChain; }

   


};


