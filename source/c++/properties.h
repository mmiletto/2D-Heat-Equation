#pragma once

// Thermal properties of known materials

double get_thermal_diffusivity(double thermal_conductivity, double density, double specific_heat_capacity)
{
    return thermal_conductivity / (density * specific_heat_capacity);
}

// known thermal diffusivity
#define STAINLESS_STEEL 4.2e-6// m^2/s