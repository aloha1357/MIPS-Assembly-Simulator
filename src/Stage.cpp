#include "Stage.h"

namespace mips {

PipelineRegister::PipelineRegister() : m_isBubble(true) {
}

bool PipelineRegister::isBubble() const {
    return m_isBubble;
}

void PipelineRegister::setBubble() {
    m_isBubble = true;
}

void PipelineRegister::reset() {
    m_isBubble = true;
}

} // namespace mips
