/*
 * Copyright (C) 2023-present ScyllaDB
 */

/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#pragma once

#include "time_window_compaction_strategy.hh"
#include "leveled_compaction_strategy.hh"
#include <utility>
#include <type_traits>
#include <variant>

namespace compaction {

using namespace sstables;

class compaction_strategy_state {
public:
    struct default_empty_state {};
    using states_variant = std::variant<default_empty_state, leveled_compaction_strategy_state, time_window_compaction_strategy_state>;
private:
    states_variant _state;
public:
    explicit compaction_strategy_state(states_variant state) : _state(std::move(state)) {
    }

    template <typename StateType>
    requires requires (StateType st) {
        { states_variant{} = st };
    }
    StateType& get() noexcept {
        return std::get<StateType>(_state);
    }

    static compaction_strategy_state make(const compaction_strategy& cs);
};

}
