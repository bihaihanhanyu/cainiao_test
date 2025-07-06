#pragma once
#include"struct_def.h"

float calculateMoney(User* user, struct Goods* goods,float base_prise)
{
   
    float userDiscount[5] = { 0.85,0.9,0.75,0.8,1.0 };
    float packageDiscount[5] = { 1.2,1.15,1.25,0.9,1.0 };
    float weightDiscount;//权重
    if (goods->weight < 1)
        weightDiscount = 1;
    else if (goods->weight < 5)
        weightDiscount = 1.2;
    else if (goods->weight < 10)
        weightDiscount = 1.5;
    else
        weightDiscount = 2;
    UserType temp_ut = user->u_type;
    PackageType temp_pt = goods->p_type;
    return base_prise * userDiscount[temp_ut] * packageDiscount[temp_pt] * weightDiscount;
}
