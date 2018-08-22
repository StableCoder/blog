/*
    Copyright (C) 2018 George Cave - gcave@stablecoder.ca

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "scalar_set.hpp"

#include <iostream>
#include <string>

enum class Special {
  Strength,
  Perception,
  Endurance,
  Charisma,
  Intelligence,
  Agility,
  Luck,
};
constexpr std::underlying_type<Special>::type cNumSpecial =
    static_cast<std::underlying_type<Special>::type>(Special::Luck) + 1;

using SpecialSet = stec::EnumeratedScalarSet<int8_t, Special, cNumSpecial>;
using SpecialSetf = stec::EnumeratedScalarSet<float, Special, cNumSpecial>;

std::string to_string(SpecialSet special) {
  std::string retVal;
  retVal.reserve(100);

  retVal = "Strength " + std::to_string(special[Special::Strength]);
  retVal += "\nPerception " + std::to_string(special[Special::Perception]);
  retVal += "\nEndurance " + std::to_string(special[Special::Endurance]);
  retVal += "\nCharisma " + std::to_string(special[Special::Charisma]);
  retVal += "\nIntelligence " + std::to_string(special[Special::Intelligence]);
  retVal += "\nAgility " + std::to_string(special[Special::Agility]);
  retVal += "\nLuck " + std::to_string(special[Special::Luck]);
  retVal += '\n';

  return retVal;
}

int main() {
  SpecialSet base(5);
  SpecialSet perks, modifiers;
  SpecialSetf multiplier(1.f);

  base[Special::Intelligence] = 8;
  base[Special::Charisma] = 2;

  perks[Special::Perception] = -2;
  perks[Special::Strength] = 1;

  modifiers[Special::Luck] = 4;

  multiplier[Special::Charisma] = 2.5f;

  SpecialSet result = (base + perks + modifiers) * multiplier;

  std::cout << "\nBase: \n"
            << to_string(base) << "\nPerks: \n"
            << to_string(perks) << "\nModifiers: \n"
            << to_string(modifiers) << "\nResult: \n"
            << to_string(result) << std::endl;
}
