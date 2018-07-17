/**
 * Copyright 2018, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "OffloadFreeList.h"
#include <iostream>

namespace FogKV {

OffloadFreeList::OffloadFreeList() {}

OffloadFreeList::~OffloadFreeList() {}

/*
 * Inserts a new element at the end of the queue.
 */
void OffloadFreeList::Push(pool_base &pop, uint64_t value) {
    transaction::exec_tx(pop, [&] {
        auto n = make_persistent<FreeLba>();

        n->lba = value;
        n->next = nullptr;

        if (_head == nullptr && _tail == nullptr) {
            _head = _tail = n;
        } else {
            _tail->next = n;
            _tail = n;
        }
    });
}

/*
 * Removes the first element in the queue.
 */
uint64_t OffloadFreeList::Pop(pool_base &pop) {
    uint64_t ret = 0;
    transaction::exec_tx(pop, [&] {
        if (_head == nullptr)
            transaction::abort(EINVAL);

        ret = _head->lba;
        auto n = _head->next;

        delete_persistent<FreeLba>(_head);
        _head = n;

        if (_head == nullptr)
            _tail = nullptr;
    });

    return ret;
}

/*
 * Prints the entire contents of the queue.
 */
void OffloadFreeList::Show(void) const {
    for (auto n = _head; n != nullptr; n = n->next)
        std::cout << n->lba << std::endl;
}

} /* namespace FogKV */
