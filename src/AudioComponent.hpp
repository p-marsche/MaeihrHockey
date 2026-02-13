//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "IComponent.hpp"

namespace mmt_gd
{
	class AudioComponent : public IComponent
	{
	public:
		AudioComponent(GameObject& go, sf::SoundBuffer& buffer)
			: IComponent(go)
			, m_sound(buffer)
		{
		}

		void playSound() 
		{
			m_sound.play();
		}

		virtual bool init()
        {
            return true;
        }

		virtual void update(float deltaTime)
		{
            return;
		}

	private:
		sf::Sound m_sound;
	};

}

