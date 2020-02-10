#include "Sence.h"

Sence::Sence()
{
}

Sence::~Sence()
{
}

void Sence::setSenceChangeFunction(std::function<void(std::shared_ptr<Sence> p)> scf)
{
	m_senceChangeFunction = scf;
}

auto Sence::getSenceChangeFunction() const
{
	return m_senceChangeFunction;
}
