#include "stdafx.h"

#include "Player.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerDashComponent.hpp"
#include "PlayerEnlargeComponent.hpp"
#include "HighDensityPassive.hpp"
#include "NoEffectPassive.hpp"
#include "HighBouncePassive.hpp"
#include "RigidBodyComponent.hpp"

#include "AssetManager.hpp"
#include "ColliderComponent.hpp"
#include "EventBus.hpp"
#include "GameObjectEvents.hpp"
#include "GameObjectFactory.hpp"
#include "InputManager.hpp"

#include <sstream>

namespace mmt_gd
{
float constexpr MARKER_PADDLE_RATIO       = 1.8f;
float constexpr INACTIVE_LINEAR_DAMPENING = 0.1f;
float constexpr ACTIVE_LINEAR_DAMPENING   = 0.2f;
float constexpr INACTIVE_RESTITUTION      = 0.9f;
float constexpr ACTIVE_RESTITUTION        = 0.7f;
int constexpr PADDLE_COUNT                = 3;

using CL = CollisionLayers;
Player::Player(const int playerIndex, sf::RenderWindow& window) :
m_listeners(),
m_paddles(),
m_moveComps(),
m_abilityComps(),
m_playerIndex(playerIndex),
m_activeIndex(1),
m_window(window)
{
    const EventBus::ListenerId
        creationListenerId = EventBus::getInstance()
                                 .addListener(GameObjectCreateEvent::Type,
                                              [this, playerIndex](const IEvent::Ptr& event)
                                              {
                                                  const auto goCreateEvent = std::static_pointer_cast<GameObjectCreateEvent>(
                                                      event);
                                                  auto              go = goCreateEvent->getData();
                                                  std::stringstream fullname(go->getId());
                                                  std::string targetname = "Player" + std::to_string(playerIndex + 1);
                                                  std::string segment;
                                                  std::getline(fullname, segment, '-');
                                                  if (segment == targetname)
                                                      addPaddle(go);
                                              });
    m_listeners.push_back(creationListenerId);

    m_activeFilterMask.categoryBits = CollisionLayers::OBJECTS;
    if (playerIndex == 0)
        m_activeFilterMask.categoryBits = m_activeFilterMask.categoryBits | CollisionLayers::PLAYER_1;
    else if (playerIndex == 1)
        m_activeFilterMask.categoryBits = m_activeFilterMask.categoryBits | CollisionLayers::PLAYER_2;

    m_activeFilterMask.maskBits     = CollisionLayers::FAKE_WALL | CollisionLayers::OBJECTS | CollisionLayers::WALL;

    m_passiveFilterMask.categoryBits = m_activeFilterMask.categoryBits;
    m_passiveFilterMask.maskBits     = CollisionLayers::FAKE_WALL | CollisionLayers::OBJECTS | CollisionLayers::WALL |
                                   CollisionLayers::PENALTY;
}

void Player::startMatch(PlayerConfig config)
{
    std::array<GameObject::Ptr, PADDLE_COUNT> temp;

    for (auto& p : m_paddles)
    {
        int length = p->getId().length();
        std::string idx = "";
        idx.push_back(p->getId().at(length - 1));
        int index = stoi(idx);
        temp[index - 1] = p;
    }
    m_paddles.clear();

    for (unsigned int i = 0; i < temp.size(); ++i)
    {
        m_paddles.push_back(temp[i]);
        setupPaddle(i, config.m_config.at(i));
    }

    setupStartingPaddle();
    
    for (auto& p : m_paddles)
    {
        auto coll = p->getComponent<ColliderComponent>();
        coll->registerOnCollisionFunction(
            [this](ColliderComponent& self, ColliderComponent& other)
            {
                GameObject& go  = self.getGameObject();
                GameObject& go2 = other.getGameObject();
                this->handleCollision(go, go2);
            });
    }
    createMarkerSprites();
}

void Player::setupStartingPaddle()
{
    m_paddles[1]->addComponent<PlayerMoveComponent>(m_moveComps[1]);
    m_paddles[1]->addComponent<IPlayerAbilityComponent>(m_abilityComps[1]);
    auto body2 = m_paddles[1]->getComponent<RigidBodyComponent>()->getB2Body();
    body2->SetLinearDamping(ACTIVE_LINEAR_DAMPENING);
    auto fix2 = m_paddles[1]->getComponent<ColliderComponent>()->getFixture();
    fix2->SetRestitution(ACTIVE_RESTITUTION);
    fix2->SetFilterData(m_activeFilterMask);
    m_activeIndex = 1;
}

void Player::setupPaddle(int index, PaddleConfig config)
{
    auto& go = m_paddles[index];
    auto rb     = go->getComponent<RigidBodyComponent>();
    auto coll   = go->getComponent<ColliderComponent>();
    auto sprite = go->getComponent<SpriteRenderComponent>();
    
    m_moveComps.push_back(std::make_shared<PlayerMoveComponent>(*go, *rb, m_playerIndex));

    auto passive = config.m_passive;
    if (passive == PaddlePassive::BOUNCY)
        m_passiveComps.push_back(std::make_shared<HighBouncePassive>(*go));
    else if (passive == PaddlePassive::HEAVY)
        m_passiveComps.push_back(std::make_shared<HighDensityPassive>(*go));
    else if (passive == PaddlePassive::NOTHING)
        m_passiveComps.push_back(std::make_shared<NoEffectPassive>(*go));
    else
        std::cerr << "Passive not found on " << go->getId() << std::endl;

    sf::Shader* cd      = AssetManager::getInstance().getFragmentShader("Cooldown");
    auto ability = config.m_ability;
    if (ability == PaddleAbility::DASH)
        m_abilityComps.push_back(std::make_shared<PlayerDashComponent>(*go, *rb, m_playerIndex, cd));
    else if (ability == PaddleAbility::ENLARGE)
        m_abilityComps.push_back(std::make_shared<PlayerEnlargeComponent>(*go, *rb, *coll, *sprite, m_playerIndex, cd));
    else
        std::cerr << "Ability not found on " << go->getId() << std::endl;

    auto abilityComp = m_abilityComps[index];
    sprite->registerShaderFuncs([abilityComp](sf::RenderStates& state)
    { 
            float       cdProg = abilityComp->getCooldownProgress(); //cd left in %
            if (cdProg < 1.0f)
            {
                sf::Shader* shader = abilityComp->getCdShader();
                shader->setUniform("progress", cdProg);
                state.shader = shader;
            }
            else
                state.shader = nullptr;
    });


    if (index != 1)
        m_passiveComps[index]->apply();
}

void Player::update(const float deltaTime)
{
    if (InputManager::getInstance().isActionJustPressed("switch", m_playerIndex))
        switchPaddle();

    for (unsigned int i = 0; i < m_abilityComps.size(); ++i)
        if (i == m_activeIndex)
            m_abilityComps[i]->updateInactive(deltaTime);

    for (unsigned int i = 0; i < m_passiveComps.size(); ++i)
        if (i == m_activeIndex)
            return;
        else if (auto bounce = std::dynamic_pointer_cast<HighBouncePassive>(m_passiveComps[i]))
            bounce->revert();
}

void Player::handleCollision(GameObject& go, GameObject& go2)
{
    for (unsigned int i = 0; i < m_paddles.size(); ++i)
    {
        if (i == m_activeIndex)
            return;
        if (m_paddles[i]->getId() == go.getId() && go2.getId() == "Puck")
            if (auto bounce = std::dynamic_pointer_cast<HighBouncePassive>(m_passiveComps[i]))
                bounce->apply();
    }
}

void Player::addPaddle(GameObject::Ptr go)
{
    std::stringstream        fullname(go->getId());
    std::string              segment;
    std::vector<std::string> parts;

    while (std::getline(fullname, segment, '-'))
    {
        parts.push_back(segment);
    }

    int         namelength = parts.at(0).length();
    std::string playerIdx  = "";
    playerIdx.push_back(parts.at(0).at(namelength - 1));
    int playerIndex = std::stoi(playerIdx) - 1;
    int paddleIndex = std::stoi(parts.at(1));

    m_paddles.push_back(go);
}

void Player::switchPaddle()
{
    deactivatePaddle();

    if (m_activeIndex == m_paddles.size() - 1)
        m_activeIndex = 0;
    else
        m_activeIndex++;

    activatePaddle();
}

void Player::activatePaddle()
{
    auto& go2 = m_paddles[m_activeIndex];
    go2->addComponent<PlayerMoveComponent>(m_moveComps[m_activeIndex]);
    go2->addComponent<IPlayerAbilityComponent>(m_abilityComps[m_activeIndex]);
    auto body2 = go2->getComponent<RigidBodyComponent>()->getB2Body();
    body2->SetLinearDamping(ACTIVE_LINEAR_DAMPENING);
    auto fix2 = go2->getComponent<ColliderComponent>()->getFixture();
    fix2->SetRestitution(ACTIVE_RESTITUTION);
    fix2->SetFilterData(m_activeFilterMask);
    m_passiveComps[m_activeIndex]->revert();

    m_activePaddleMarker[m_activeIndex]->setVisibility(true);
}

void Player::deactivatePaddle()
{
    m_activePaddleMarker[m_activeIndex]->setVisibility(false);

    auto& go1   = m_paddles[m_activeIndex];
    auto body1 = go1->getComponent<RigidBodyComponent>()->getB2Body();
    body1->SetLinearDamping(INACTIVE_LINEAR_DAMPENING);
    auto fix1 = go1->getComponent<ColliderComponent>()->getFixture();
    fix1->SetRestitution(INACTIVE_RESTITUTION);
    fix1->SetFilterData(m_passiveFilterMask);
    auto move = go1->getComponent<PlayerMoveComponent>();
    go1->removeComponent(move);
    go1->removeComponent(m_abilityComps[m_activeIndex]);
    m_passiveComps[m_activeIndex]->apply();
}

void Player::shutdown()
{
    for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
    {
        EventBus::getInstance().removeListener(*it);
    }

    m_listeners.clear();
}

void Player::createMarkerSprites()
{
    auto& paddleScale = m_paddles[0]->getComponent<SpriteRenderComponent>()->getSprite().getScale();

    for (unsigned int i = 0; i < m_paddles.size(); i++)
    {
        auto marker = m_paddles[i]->addComponent<SpriteRenderComponent>(
                *m_paddles[i],
                m_window,
                AssetManager::getInstance().getTexture("Selected Marker"),
                "GameObjects",
                sf::IntRect(0, 0, 0, 0));
        auto textureSize = AssetManager::getInstance().getTexture("Selected Marker").getSize();
        marker->getSprite().setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        marker->setScale( MARKER_PADDLE_RATIO * paddleScale);
        marker->setVisibility(false);
        marker->init();
        m_activePaddleMarker.push_back(marker);
    }
    m_activePaddleMarker[m_activeIndex]->setVisibility(true);
}
} // namespace mmt_gd
