/**
 * Copyright 2017-2018, Intel Corporation
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

#pragma once

#include <vector>
#include <asio/io_service.hpp>

#include <FogKV/Types.h>

namespace FogKV {

struct AllocOptions {
};

struct UpdateOptions {
};

struct PutOptions {
};

struct GetOptions {
};

struct KeyFieldDescriptor {
	KeyFieldDescriptor() : Size(0) {}
	size_t Size;
};

struct KeyDescriptor {

	size_t nfields() const
	{
		return _fields.size();
	}

	void field(size_t idx, size_t size)
	{
		if (nfields() <= idx)
			_fields.resize(idx + 1);

		_fields[idx].Size = size;
	}
	
	KeyFieldDescriptor field(size_t idx) const
	{
		if (nfields() <= idx)
			return KeyFieldDescriptor();

		return _fields[idx];
	}

	void clear()
	{
		_fields.clear();
	}

	std::vector<KeyFieldDescriptor> _fields;
};

struct RuntimeOptions {
	void io_service(asio::io_service *io_service)
	{
		_io_service = io_service;
	}

	asio::io_service *io_service()
	{
		return _io_service;
	}

	asio::io_service *_io_service;
};

struct DhtOptions {
	unsigned short Port;
	NodeId Id;
};

struct Options {
public:
	Options() { }
	Options(const std::string &path);

	KeyDescriptor Key;
	RuntimeOptions Runtime;
	DhtOptions Dht;

	// TODO move to struct ?
	unsigned short Port;
};

} // namespace FogKV