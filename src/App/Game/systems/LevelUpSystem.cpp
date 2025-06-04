#include "LevelUpSystem.h"

#include "AttributeSystem.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "ExperienceSystem.h"
#include "Game.h"
#include "InputId.h"

char constexpr ATTRIBUTE_CHOICES[ATTRIBUTES + 1]{
    '0', //* NONE
    'v', //* Vitality
    't', //* Strength
    'e', //* Defence
    'g', //* Agility
};

[[nodiscard]]
Hero const& levelUpHero(
    Hero& hero,
    char attributeChoice
)
{
    switch ( attributeChoice )
    {
        default:
            return hero;

        case 'V':
        case 'v':
        {
            ++hero.attributes.vitality;

            break;
        }

        case 'T':
        case 't':
        {
            ++hero.attributes.strength;

            break;
        }

        case 'E':
        case 'e':
        {
            ++hero.attributes.defense;

            break;
        }

        case 'G':
        case 'g':
        {
            ++hero.attributes.agility;

            break;
        }
    }

    //* NOTE: Deactivate health.base increase for now
    // ExperienceSystem::updateStats( hero.health );

    AttributeSystem::updateStats(
        hero.health,
        hero.energy,
        hero.attributes
    );

    hero.health.current = hero.health.maximum;

    if ( ( AttributesModule::totalPoints( hero.attributes ) - ( ATTRIBUTES * BASE_POINTS ) ) < hero.experience.level )
    {
        return hero;
    }
    snx::EventDispatcher::notify( EventId::LEVELED_UP );

    return hero;
}

namespace LevelUpSystem
{
    Game const& update(
        Game& game,
        InputId currentInputId
    )
    {
        char attributeChoice{ 0 };

        switch ( currentInputId )
        {
            default:
                attributeChoice = GetCharPressed();

                break;

            case InputId::ACT_DOWN:
            {
                ++game.selectedAttribute;

                break;
            }

            case InputId::ACT_UP:
            {
                --game.selectedAttribute;

                break;
            }

            case InputId::ACT_IN_PLACE:
            {
                if ( !attributeChoice )
                {
                    attributeChoice = ATTRIBUTE_CHOICES[game.selectedAttribute];
                }

                break;
            }
        }

        if ( game.selectedAttribute < 0 )
        {
            game.selectedAttribute = ATTRIBUTES;
        }
        else if ( game.selectedAttribute > ATTRIBUTES )
        {
            game.selectedAttribute = 0;
        }

        game.hero = levelUpHero(
            game.hero,
            attributeChoice
        );

        return game;
    }

}
