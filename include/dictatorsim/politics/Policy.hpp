#pragma once
namespace ds {
    struct Policy {
        int incomeTax=1500;
        int welfareRate=500;
        double police=.4;
        bool emergency=false;
        bool censorship=false, organizationBanned=false, protestAllowed=false;
    };
}
