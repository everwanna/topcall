LOCAL_PATH := $(call my-dir)
ARMWB	:= amrwb

include $(CLEAR_VARS)
LOCAL_MODULE := libamrwb
LOCAL_SRC_FILES := amrwb_jni.c	\
		$(ARMWB)/dec_acelp.c \
		$(ARMWB)/dec_dtx.c	\
		$(ARMWB)/dec_gain.c	\
		$(ARMWB)/dec_if.c	\
		$(ARMWB)/dec_lpc.c	\
		$(ARMWB)/dec_main.c	\
		$(ARMWB)/dec_rom.c	\
		$(ARMWB)/dec_util.c	\
	  	$(ARMWB)/if_rom.c	
	  	

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(ARMWB)/
LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS += -DHAVE_NEON=1 
LOCAL_CFLAGS +=  -O3 -marm -UHAVE_CONFIG_H
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)




SPEEX	:= speex
include $(CLEAR_VARS)
LOCAL_MODULE    := libspeex
LOCAL_SRC_FILES := speex_jni.c \
		$(SPEEX)/libspeex/bits.c \
		$(SPEEX)/libspeex/buffer.c \
		$(SPEEX)/libspeex/cb_search.c \
		$(SPEEX)/libspeex/exc_10_16_table.c \
		$(SPEEX)/libspeex/exc_10_32_table.c \
		$(SPEEX)/libspeex/exc_20_32_table.c \
		$(SPEEX)/libspeex/exc_5_256_table.c \
		$(SPEEX)/libspeex/exc_5_64_table.c \
		$(SPEEX)/libspeex/exc_8_128_table.c \
		$(SPEEX)/libspeex/fftwrap.c \
		$(SPEEX)/libspeex/filterbank.c \
		$(SPEEX)/libspeex/filters.c \
		$(SPEEX)/libspeex/gain_table.c \
		$(SPEEX)/libspeex/gain_table_lbr.c \
		$(SPEEX)/libspeex/hexc_10_32_table.c \
		$(SPEEX)/libspeex/hexc_table.c \
		$(SPEEX)/libspeex/high_lsp_tables.c \
		$(SPEEX)/libspeex/jitter.c \
		$(SPEEX)/libspeex/kiss_fft.c \
		$(SPEEX)/libspeex/kiss_fftr.c \
		$(SPEEX)/libspeex/lpc.c \
		$(SPEEX)/libspeex/lsp.c \
		$(SPEEX)/libspeex/lsp_tables_nb.c \
		$(SPEEX)/libspeex/ltp.c \
		$(SPEEX)/libspeex/mdf.c \
		$(SPEEX)/libspeex/modes.c \
		$(SPEEX)/libspeex/modes_wb.c \
		$(SPEEX)/libspeex/nb_celp.c \
		$(SPEEX)/libspeex/preprocess.c \
		$(SPEEX)/libspeex/quant_lsp.c \
		$(SPEEX)/libspeex/resample.c \
		$(SPEEX)/libspeex/sb_celp.c \
		$(SPEEX)/libspeex/scal.c \
		$(SPEEX)/libspeex/smallft.c \
		$(SPEEX)/libspeex/speex.c \
		$(SPEEX)/libspeex/speex_callbacks.c \
		$(SPEEX)/libspeex/speex_header.c \
		$(SPEEX)/libspeex/stereo.c \
		$(SPEEX)/libspeex/vbr.c \
		$(SPEEX)/libspeex/vq.c \
		$(SPEEX)/libspeex/window.c \

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SPEEX)/include
LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H -I$(LOCAL_PATH)/$(SPEEX)/include
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)




SILK = silk
include $(CLEAR_VARS)
LOCAL_MODULE := libsilk
LOCAL_SRC_FILES := silk_jni.c		\
		$(SILK)/src/SKP_Silk_A2NLSF.c \
		$(SILK)/src/SKP_Silk_CNG.c \
		$(SILK)/src/SKP_Silk_HP_variable_cutoff_FIX.c \
		$(SILK)/src/SKP_Silk_LBRR_reset.c \
		$(SILK)/src/SKP_Silk_LPC_inv_pred_gain.c \
		$(SILK)/src/SKP_Silk_LPC_stabilize.c \
		$(SILK)/src/SKP_Silk_LPC_synthesis_filter.c \
		$(SILK)/src/SKP_Silk_LPC_synthesis_order16.c \
		$(SILK)/src/SKP_Silk_LP_variable_cutoff.c \
		$(SILK)/src/SKP_Silk_LSF_cos_table.c \
		$(SILK)/src/SKP_Silk_LTP_analysis_filter_FIX.c \
		$(SILK)/src/SKP_Silk_LTP_scale_ctrl_FIX.c \
		$(SILK)/src/SKP_Silk_MA.c \
		$(SILK)/src/SKP_Silk_NLSF2A.c \
		$(SILK)/src/SKP_Silk_NLSF2A_stable.c \
		$(SILK)/src/SKP_Silk_NLSF_MSVQ_decode.c \
		$(SILK)/src/SKP_Silk_NLSF_MSVQ_encode_FIX.c \
		$(SILK)/src/SKP_Silk_NLSF_VQ_rate_distortion_FIX.c \
		$(SILK)/src/SKP_Silk_NLSF_VQ_sum_error_FIX.c \
		$(SILK)/src/SKP_Silk_NLSF_VQ_weights_laroia.c \
		$(SILK)/src/SKP_Silk_NLSF_stabilize.c \
		$(SILK)/src/SKP_Silk_NSQ.c \
		$(SILK)/src/SKP_Silk_NSQ_del_dec.c \
		$(SILK)/src/SKP_Silk_PLC.c \
		$(SILK)/src/SKP_Silk_VAD.c \
		$(SILK)/src/SKP_Silk_VQ_nearest_neighbor_FIX.c \
		$(SILK)/src/SKP_Silk_allpass_int.c \
		$(SILK)/src/SKP_Silk_ana_filt_bank_1.c \
		$(SILK)/src/SKP_Silk_apply_sine_window.c \
		$(SILK)/src/SKP_Silk_array_maxabs.c \
		$(SILK)/src/SKP_Silk_autocorr.c \
		$(SILK)/src/SKP_Silk_biquad.c \
		$(SILK)/src/SKP_Silk_biquad_alt.c \
		$(SILK)/src/SKP_Silk_burg_modified.c \
		$(SILK)/src/SKP_Silk_bwexpander.c \
		$(SILK)/src/SKP_Silk_bwexpander_32.c \
		$(SILK)/src/SKP_Silk_code_signs.c \
		$(SILK)/src/SKP_Silk_control_codec_FIX.c \
		$(SILK)/src/SKP_Silk_corrMatrix_FIX.c \
		$(SILK)/src/SKP_Silk_create_init_destroy.c \
		$(SILK)/src/SKP_Silk_dec_API.c \
		$(SILK)/src/SKP_Silk_decode_core.c \
		$(SILK)/src/SKP_Silk_decode_frame.c \
		$(SILK)/src/SKP_Silk_decode_indices_v4.c \
		$(SILK)/src/SKP_Silk_decode_parameters.c \
		$(SILK)/src/SKP_Silk_decode_parameters_v4.c \
		$(SILK)/src/SKP_Silk_decode_pulses.c \
		$(SILK)/src/SKP_Silk_decoder_set_fs.c \
		$(SILK)/src/SKP_Silk_detect_SWB_input.c \
		$(SILK)/src/SKP_Silk_enc_API.c \
		$(SILK)/src/SKP_Silk_encode_frame_FIX.c \
		$(SILK)/src/SKP_Silk_encode_parameters.c \
		$(SILK)/src/SKP_Silk_encode_parameters_v4.c \
		$(SILK)/src/SKP_Silk_encode_pulses.c \
		$(SILK)/src/SKP_Silk_find_LPC_FIX.c \
		$(SILK)/src/SKP_Silk_find_LTP_FIX.c \
		$(SILK)/src/SKP_Silk_find_pitch_lags_FIX.c \
		$(SILK)/src/SKP_Silk_find_pred_coefs_FIX.c \
		$(SILK)/src/SKP_Silk_gain_quant.c \
		$(SILK)/src/SKP_Silk_init_encoder_FIX.c \
		$(SILK)/src/SKP_Silk_inner_prod_aligned.c \
		$(SILK)/src/SKP_Silk_interpolate.c \
		$(SILK)/src/SKP_Silk_k2a.c \
		$(SILK)/src/SKP_Silk_k2a_Q16.c \
		$(SILK)/src/SKP_Silk_lin2log.c \
		$(SILK)/src/SKP_Silk_log2lin.c \
		$(SILK)/src/SKP_Silk_lowpass_int.c \
		$(SILK)/src/SKP_Silk_lowpass_short.c \
		$(SILK)/src/SKP_Silk_noise_shape_analysis_FIX.c \
		$(SILK)/src/SKP_Silk_pitch_analysis_core.c \
		$(SILK)/src/SKP_Silk_pitch_est_tables.c \
		$(SILK)/src/SKP_Silk_prefilter_FIX.c \
		$(SILK)/src/SKP_Silk_process_NLSFs_FIX.c \
		$(SILK)/src/SKP_Silk_process_gains_FIX.c \
		$(SILK)/src/SKP_Silk_pulses_to_bytes.c \
		$(SILK)/src/SKP_Silk_quant_LTP_gains_FIX.c \
		$(SILK)/src/SKP_Silk_range_coder.c \
		$(SILK)/src/SKP_Silk_regularize_correlations_FIX.c \
		$(SILK)/src/SKP_Silk_resample_1_2.c \
		$(SILK)/src/SKP_Silk_resample_1_2_coarse.c \
		$(SILK)/src/SKP_Silk_resample_1_2_coarsest.c \
		$(SILK)/src/SKP_Silk_resample_1_3.c \
		$(SILK)/src/SKP_Silk_resample_2_1_coarse.c \
		$(SILK)/src/SKP_Silk_resample_2_3.c \
		$(SILK)/src/SKP_Silk_resample_2_3_coarse.c \
		$(SILK)/src/SKP_Silk_resample_2_3_coarsest.c \
		$(SILK)/src/SKP_Silk_resample_2_3_rom.c \
		$(SILK)/src/SKP_Silk_resample_3_1.c \
		$(SILK)/src/SKP_Silk_resample_3_2.c \
		$(SILK)/src/SKP_Silk_resample_3_2_rom.c \
		$(SILK)/src/SKP_Silk_resample_3_4.c \
		$(SILK)/src/SKP_Silk_resample_4_3.c \
		$(SILK)/src/SKP_Silk_residual_energy16_FIX.c \
		$(SILK)/src/SKP_Silk_residual_energy_FIX.c \
		$(SILK)/src/SKP_Silk_scale_copy_vector16.c \
		$(SILK)/src/SKP_Silk_scale_vector.c \
		$(SILK)/src/SKP_Silk_schur.c \
		$(SILK)/src/SKP_Silk_schur64.c \
		$(SILK)/src/SKP_Silk_shell_coder.c \
		$(SILK)/src/SKP_Silk_sigm_Q15.c \
		$(SILK)/src/SKP_Silk_solve_LS_FIX.c \
		$(SILK)/src/SKP_Silk_sort.c \
		$(SILK)/src/SKP_Silk_sum_sqr_shift.c \
		$(SILK)/src/SKP_Silk_tables_LTP.c \
		$(SILK)/src/SKP_Silk_tables_NLSF_CB0_10.c \
		$(SILK)/src/SKP_Silk_tables_NLSF_CB0_16.c \
		$(SILK)/src/SKP_Silk_tables_NLSF_CB1_10.c \
		$(SILK)/src/SKP_Silk_tables_NLSF_CB1_16.c \
		$(SILK)/src/SKP_Silk_tables_gain.c \
		$(SILK)/src/SKP_Silk_tables_other.c \
		$(SILK)/src/SKP_Silk_tables_pitch_lag.c \
		$(SILK)/src/SKP_Silk_tables_pulses_per_block.c \
		$(SILK)/src/SKP_Silk_tables_sign.c \
		$(SILK)/src/SKP_Silk_tables_type_offset.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SILK)/interface 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SILK)/src
LOCAL_CFLAGS = -DFIXED_POINT -DEXPORT="" -UHAVE_CONFIG_H -I$(LOCAL_PATH)/$(SILK)/interface
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)