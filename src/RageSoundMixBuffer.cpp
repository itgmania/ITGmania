#include "global.h"
#include "RageSoundMixBuffer.h"
#include "RageUtil.h"
#include "PrefsManager.h"
#include "RageLog.h"

#include <cstdint>
#include <cstdlib>

static size_t size_in_bytes_ = 70560;

static void GetBufferSizeFromPreference() {
	size_in_bytes_ = static_cast<size_t>(PREFSMAN->m_AudioBufferSize.Get());
	if (size_in_bytes_ < 4409) {
		LOG->Warn("The value of AudioBufferSize is too small (%zu) - changing the buffer size to 4410.", size_in_bytes_);
		size_in_bytes_ = 4410;
	}
}

RageSoundMixBuffer::RageSoundMixBuffer() {
	GetBufferSizeFromPreference();
	LOG->Info("Audio mix buffer size: %.1f seconds @ 44.1kHz (%zu bytes)", static_cast<float>(size_in_bytes_) / (44100.0f * sizeof(float)), size_in_bytes_);
	m_iBufSize = size_in_bytes_;
	m_iBufUsed = 0;
	m_iOffset = 0;
	m_pMixbuf = static_cast<float*>(std::malloc(size_in_bytes_));
	if (!(m_pMixbuf)) {
		FAIL_M("Failed to allocate memory for the audio mix buffer");
	}
	std::memset(m_pMixbuf, 0, size_in_bytes_);
}

RageSoundMixBuffer::~RageSoundMixBuffer() {
	std::free(m_pMixbuf);
}

// Initialize any unused part of the buffer with zeroes, if it exists.
void RageSoundMixBuffer::Extend(unsigned iSamples) noexcept {
	const int_fast64_t realsize = iSamples + m_iOffset;

	if( m_iBufUsed < realsize )
	{
		std::memset(m_pMixbuf + m_iBufUsed, 0, (realsize - m_iBufUsed) * sizeof(float));
		m_iBufUsed = realsize;
	}
}

void RageSoundMixBuffer::write( const float *pBuf, unsigned iSize, int iSourceStride, int iDestStride ) noexcept {
	if( iSize == 0 ) {
		return;
	}

	// iSize = 3, iDestStride = 2 uses 4 frames.  Don't allocate the stride of the last sample.
	Extend( iSize * iDestStride - (iDestStride-1) );

	// Scale volume and add.
	float *pDestBuf = m_pMixbuf+m_iOffset;

	while( iSize ) {
		*pDestBuf += *pBuf;
		pBuf += iSourceStride;
		pDestBuf += iDestStride;
		--iSize;
	}
}

/*
 * Example usage:
 * rsmb.Reinitialize(); // No argument, calls GetBufferSizeFromPreference()
 * rsmb.Reinitialize(17640); // With arugment, updates size_in_bytes_ to be 17640
 */
void RageSoundMixBuffer::Reinitialize(unsigned new_size = 0) {
	// Check if we were given a buffer size
	size_in_bytes_ = (new_size > 0) ? new_size : size_in_bytes_;
	if (new_size == 0) { GetBufferSizeFromPreference(); }
	// Set up the temporary buffer, switch, & free old memory
	float* temp_buf = static_cast<float*>(std::malloc(size_in_bytes_));
	if (!(temp_buf)) {
		FAIL_M("Failed to allocate memory for the audio mix buffer");
	}
	std::memset(temp_buf, 0, size_in_bytes_);
	float* old_buf = m_pMixbuf;
	m_pMixbuf = temp_buf;
	m_iBufSize = size_in_bytes_;
	m_iBufUsed = 0;
	m_iOffset = 0;
	std::free(old_buf);
	LOG->Info("Audio mix buffer size: %.1f seconds @ 44.1kHz (%zu bytes)", static_cast<float>(size_in_bytes_) / (44100.0f * sizeof(float)), size_in_bytes_);
}

void RageSoundMixBuffer::read_deinterlace( float **pBufs, int channels ) noexcept {
	for( unsigned i = 0; i < m_iBufUsed / channels; ++i ) {
		for( int ch = 0; ch < channels; ++ch ) {
			pBufs[ch][i] = m_pMixbuf[channels * i + ch];
		}
	}
	m_iBufUsed = 0;
}

/* New code by sukibaby, 2024 - rewritten for fixed buffer size. */

/*
 * Copyright (c) 2002-2004 Glenn Maynard
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
