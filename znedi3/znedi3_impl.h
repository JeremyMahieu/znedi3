#pragma once

#ifndef ZNEDI3_ZNEDI3_IMPL_H_
#define ZNEDI3_ZNEDI3_IMPL_H_

#include <cstddef>
#include <memory>
#include "kernel.h"
#include "znedi3.h"

struct znedi3_filter {
protected:
	~znedi3_filter() = default;
};

namespace znedi3 {

enum class PixelType {
	BYTE,
	WORD,
	HALF,
	FLOAT,
};

class NNEDI3Weights;

class znedi3_filter : public ::znedi3_filter {
	std::unique_ptr<Prescreener> m_prescreener;
	std::unique_ptr<Predictor> m_predictor;
	interpolate_func m_interpolate_func;
	pixel_io_func m_pixel_load_func;
	pixel_io_func m_pixel_store_func;

	PixelType m_type;
	CPUClass m_cpu;
public:
	znedi3_filter(const NNEDI3Weights &weights, const znedi3_filter_params &params);

	size_t get_tmp_size(unsigned width, unsigned height) const;

	void process(unsigned width, unsigned height, const void *src, ptrdiff_t src_stride, void *dst, ptrdiff_t dst_stride, void *tmp, unsigned parity) const;
};

class znedi3_filter2 : public ::znedi3_filter {
	struct filters;
	struct data;

	std::unique_ptr<filters> m_filters;
	std::unique_ptr<data> m_data_t;
	std::unique_ptr<data> m_data_b;

	std::unique_ptr<filters> setup_filters(const NNEDI3Weights &weights, const znedi3_filter_params &params, unsigned width, unsigned height);

	std::unique_ptr<data> setup_graph(const NNEDI3Weights &weights, const znedi3_filter_params &params, unsigned width, unsigned height, bool parity);
public:
	znedi3_filter2(const NNEDI3Weights &weights, const znedi3_filter_params &params, unsigned width, unsigned height);

	~znedi3_filter2();

	size_t get_tmp_size() const;

	void process(const void *src, ptrdiff_t src_stride, void *dst, ptrdiff_t dst_stride, void *tmp, bool parity) const;
};

} // namespace znedi3

#endif // ZNEDI3_ZNEDI3_IMPL_H_
