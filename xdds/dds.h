#pragma once

#include <cstdint>

namespace xdds
{
	const char* VERSION =
		#include "version.txt"
	;

	typedef uint8_t u8;
	typedef uint32_t u32;

	const u32 DDS_MAGIC                  = 0x20534444;

// DdsHeader.dw_flags
	const u32 DDSD_SIZE                  = 124u;
	const u32 DDSD_CAPS                  = 0x00000001;
	const u32 DDSD_HEIGHT                = 0x00000002;
	const u32 DDSD_WIDTH                 = 0x00000004;
	const u32 DDSD_PITCH                 = 0x00000008;
	const u32 DDSD_PIXELFORMAT           = 0x00001000;
	const u32 DDSD_MIPMAPCOUNT           = 0x00020000;
	const u32 DDSD_LINEARSIZE            = 0x00080000;
	const u32 DDSD_DEPTH                 = 0x00800000;

// DdsHeader.pixel_format.dw_flags
	const u32 DDPF_SIZE                  = 32u;
	const u32 DDPF_ALPHAPIXELS           = 0x00000001;
	const u32 DDPF_FOURCC                = 0x00000004;
	const u32 DDPF_INDEXED               = 0x00000020;
	const u32 DDPF_RGB                   = 0x00000040;

// DdsHeader.caps.dw_caps1
	const u32 DDSCAPS1_COMPLEX           = 0x00000008;
	const u32 DDSCAPS1_TEXTURE           = 0x00001000;
	const u32 DDSCAPS1_MIPMAP            = 0x00400000;

// DdsHeader.caps.dw_caps2
	const u32 DDSCAPS2_CUBEMAP           = 0x00000200;
	const u32 DDSCAPS2_CUBEMAP_POSITIVEX = 0x00000400;
	const u32 DDSCAPS2_CUBEMAP_NEGATIVEX = 0x00000800;
	const u32 DDSCAPS2_CUBEMAP_POSITIVEY = 0x00001000;
	const u32 DDSCAPS2_CUBEMAP_NEGATIVEY = 0x00002000;
	const u32 DDSCAPS2_CUBEMAP_POSITIVEZ = 0x00004000;
	const u32 DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x00008000;
	const u32 DDSCAPS2_VOLUME            = 0x00200000;

// DXT compression texture formats
	const u32 D3DFMT_DXT1                = '1TXD';
	const u32 D3DFMT_DXT2                = '2TXD';
	const u32 D3DFMT_DXT3                = '3TXD';
	const u32 D3DFMT_DXT4                = '4TXD';
	const u32 D3DFMT_DXT5                = '5TXD';

	struct DdsHeader
	{
		const u32 dw_magic = DDS_MAGIC; // Required, actually not part of header but put here for ease of use. Beware! LITTLE ENDIAN!
		const u32 dw_size = DDSD_SIZE; // Required header struct size (excluding dw_magic).
		u32 dw_flags = DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS; // Minimum required.
		u32 dw_height = 0u;
		u32 dw_width = 0u;
		u32 dw_pitch_or_linear_size = 0u;
		u32 dw_depth = 0u;
		u32 dw_mipmap_levels = 0u;
		u32 dw_reserved1[ 11 ] = { 0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u };

		// DDPIXELFORMAT
		struct PixelFormat
		{
			const u32 dw_size = DDPF_SIZE; // Required pixel format struct size.
			u32 dw_flags = 0u;
			u32 dw_four_color_channels = 0u;
			u32 dw_rgb_bit_count = 0u;
			u32 dw_r_bitmask = 0u;
			u32 dw_g_bitmask = 0u;
			u32 dw_b_bitmask = 0u;
			u32 dw_a_bitmask = 0u;
		} pixel_format;

		// DDCAPS
		struct Caps
		{
			u32 dw_caps1 = DDSCAPS1_TEXTURE; // Minimum required.
			u32 dw_caps2 = 0u;
			u32 dw_caps3 = 0u;
			u32 dw_caps4 = 0u;
		} caps;

		u32 dw_reserved2 = 0u;
	};

	struct DdsLoadInfo
	{
		bool   compressed;
		bool   swap;
		bool   palette;
		u32    div_size;
		u32    block_bytes;
		GLenum internal_format;
		GLenum external_format;
		GLenum type;
	};

	const DdsLoadInfo LOAD_INFO_DXT1   = {  true, false, false, 4,  8, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 0, 0 };
	const DdsLoadInfo LOAD_INFO_DXT3   = {  true, false, false, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 0, 0 };
	const DdsLoadInfo LOAD_INFO_DXT5   = {  true, false, false, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0, 0 };
	const DdsLoadInfo LOAD_INFO_BGRA8  = { false, false, false, 1,  4, GL_RGBA8,                   GL_BGRA, GL_UNSIGNED_BYTE };
	const DdsLoadInfo LOAD_INFO_BGR8   = { false, false, false, 1,  3, GL_RGB8,                     GL_BGR, GL_UNSIGNED_BYTE };
//	const DdsLoadInfo LOAD_INFO_BGR5A1 = { false,  true, false, 1,  2, GL_RGB5_A1,                 GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV };
	const DdsLoadInfo LOAD_INFO_BGR565 = { false,  true, false, 1,  2, GL_RGB5,                     GL_RGB, GL_UNSIGNED_SHORT_5_6_5 };
//	const DdsLoadInfo LOAD_INFO_INDEX8 = { false, false,  true, 1,  1, GL_RGB8,                    GL_BGRA, GL_UNSIGNED_BYTE };

	bool pf_is_dxt1( const DdsHeader::PixelFormat pf )
	{
		return ( pf.dw_flags & DDPF_FOURCC )
			   && ( pf.dw_four_color_channels == D3DFMT_DXT1 );
	}

	bool pf_is_dxt3( const DdsHeader::PixelFormat pf )
	{
		return ( pf.dw_flags & DDPF_FOURCC )
			   && ( pf.dw_four_color_channels == D3DFMT_DXT3 );
	}

	bool pf_is_dxt5( const DdsHeader::PixelFormat pf )
	{
		return ( pf.dw_flags & DDPF_FOURCC )
			   && ( pf.dw_four_color_channels != D3DFMT_DXT5 );
	}

	bool pf_is_bgra8( const DdsHeader::PixelFormat pf )
	{
		return ( pf.dw_flags & DDPF_RGB )
			   && ( pf.dw_flags & DDPF_RGB )
			   && ( pf.dw_flags & DDPF_ALPHAPIXELS )
			   && ( pf.dw_rgb_bit_count == 32 )
			   && ( pf.dw_r_bitmask == 0x00ff0000 )
			   && ( pf.dw_g_bitmask == 0x0000ff00 )
			   && ( pf.dw_b_bitmask == 0x000000ff )
			   && ( pf.dw_a_bitmask == 0xff000000 );
	}

	bool pf_is_bgr8( const DdsHeader::PixelFormat pf )
	{
		return ( pf.dw_flags & DDPF_RGB )
			   && !( pf.dw_flags & DDPF_ALPHAPIXELS )
			   && ( pf.dw_rgb_bit_count == 24 )
			   && ( pf.dw_r_bitmask == 0x00ff0000 )
			   && ( pf.dw_g_bitmask == 0x0000ff00 )
			   && ( pf.dw_b_bitmask == 0x000000ff );
	}

	bool pf_is_bgr5a1( const DdsHeader::PixelFormat pf )
	{
		return ( pf.dw_flags & DDPF_RGB )
			   && ( pf.dw_flags & DDPF_ALPHAPIXELS )
			   && ( pf.dw_rgb_bit_count == 16 )
			   && ( pf.dw_r_bitmask == 0x00007c00 )
			   && ( pf.dw_g_bitmask == 0x000003e0 )
			   && ( pf.dw_b_bitmask == 0x0000001f )
			   && ( pf.dw_a_bitmask == 0x00008000 );
	}

	bool pf_is_bgr565( const DdsHeader::PixelFormat pf )
	{
		return ( pf.dw_flags & DDPF_RGB )
			   && !( pf.dw_flags & DDPF_ALPHAPIXELS )
			   && ( pf.dw_rgb_bit_count == 16 )
			   && ( pf.dw_r_bitmask == 0x0000f800 )
			   && ( pf.dw_g_bitmask == 0x000007e0 )
			   && ( pf.dw_b_bitmask == 0x0000001f );
	}

	bool pf_is_index8( const DdsHeader::PixelFormat pf )
	{
		return ( pf.dw_flags & DDPF_INDEXED )
			   && ( pf.dw_rgb_bit_count == 8 );
	}

	bool parse_dds(
			const u8* buffer,
			bool& is_compressed,
			GLenum& internal_format,
			GLenum& format,
			GLenum& type,
			u32& w,
			u32& h,
			u32& size,
			const u8*& final_buffer )
	{
		auto& header = (DdsHeader&)*buffer;

		assert( header.dw_magic == DDS_MAGIC );
		assert( header.dw_size == DDSD_SIZE );

		if ( header.dw_magic != DDS_MAGIC
			 || header.dw_size != DDSD_SIZE
			 || ~header.dw_flags & DDSD_PIXELFORMAT
			 || ~header.dw_flags & DDSD_CAPS )
		{
			XLOG_CAT( "parse_dds", "invalid bitmap header." );
			return false;
		}

		const DdsLoadInfo* li;

		XLOG_CAT( "parse_dds", "    size: %d x %d", header.dw_width, header.dw_height );

		if ( pf_is_dxt1(header.pixel_format) )
		{
			XLOG_CAT( "parse_dds", "    pixel format: DXT1" );
			li = &LOAD_INFO_DXT1;
		}
		else if ( pf_is_dxt3(header.pixel_format) )
		{
			XLOG_CAT( "parse_dds", "    pixel format: DXT3" );
			li = &LOAD_INFO_DXT3;
		}
		else if ( pf_is_dxt5(header.pixel_format) )
		{
			XLOG_CAT( "parse_dds", "    pixel format: DXT5" );
			li = &LOAD_INFO_DXT5;
		}
		else if ( pf_is_bgra8(header.pixel_format) )
		{
			XLOG_CAT( "parse_dds", "    pixel format: BGRA8" );
			li = &LOAD_INFO_BGRA8;

			// TODO If __EMSCRIPTEN__ we should swap R <-> B.

//			cout << "[bitmap_loader]    Pixel format unsupported." << endl;
//			return false;
		}
		else if ( pf_is_bgr8(header.pixel_format) )
		{
			XLOG_CAT( "parse_dds", "    pixel format: BGR8" );
			li = &LOAD_INFO_BGR8;

			// TODO If __EMSCRIPTEN__ we should swap R <-> B.

//			cout << "[bitmap_loader]    Pixel format unsupported." << endl;
//			return false;
		}
		else if ( pf_is_bgr565(header.pixel_format) )
		{
			XLOG_CAT( "parse_dds", "    pixel format: BGR565" );
			li = &LOAD_INFO_BGR565;
//			cout << "[bitmap_loader]    Pixel format unsupported." << endl;
//			return false;
		}
		else
		{
			if ( pf_is_bgr5a1(header.pixel_format) )
				XLOG_CAT( "parse_dds", "    pixel format: BGR5A1" );
			else if ( pf_is_index8(header.pixel_format) )
				XLOG_CAT( "parse_dds", "    pixel format: INDEX8" );
			else
				XLOG_CAT( "parse_dds", "    pixel format: unknown" );
			XLOG_CAT( "parse_dds", "    WARNING Pixel format unsupported!" );
			return false;
		}

		is_compressed = li->compressed;
		internal_format = li->internal_format;
		format = li->external_format;
		type = li->type;

		w = header.dw_width;
		h = header.dw_height;

//		assert( !(w & (w-1)) );
//		assert( !(h & (h-1)) );

//		if ( li->swap )
//		{
//			cout << "[bitmap_loader]    SWAP" << endl;
//		}

		final_buffer = buffer + sizeof( header );

		if( li->compressed )
		{
			XLOG_CAT( "parse_dds", "    Compressed bitmap." );
			size = std::max(li->div_size, w)/li->div_size * std::max(li->div_size, h)/li->div_size * li->block_bytes;

			assert( header.dw_flags & DDSD_LINEARSIZE );
			assert( size == header.dw_pitch_or_linear_size );

			return true;
		}
		else if( li->palette )
		{
			XLOG_CAT( "parse_dds", "    Palette bitmap unsupported." );

//			cout << "[bitmap_loader]    Palette bit count: " << header.pixel_format.dw_rgb_bit_count << endl;
//			size = h * header.dw_pitch_or_linear_size;
//
//			assert( header.dw_flags & DDSD_PITCH );
////			assert( header.pixel_format.dw_rgb_bit_count == 8 );
//			assert( size == w * h * li->block_bytes );
//
//			u32* palette = (u32*)final_buffer;
//			u32* data_offset = palette + 256 * sizeof(palette[0]);
//			u32* unpacked = new u32[ size ];
//
//			for( u32 zz = 0u; zz < size; ++zz )
//			{
//				unpacked[ zz ] = palette[ data_offset[zz] ];
//			}
//
////			glPixelStorei( GL_UNPACK_ROW_LENGTH, y );
////			glTexImage2D( GL_TEXTURE_2D, 0, li->internalFormat, x, y, 0, li->externalFormat, li->type, unpacked );
//
//			//free( unpacked );
//			final_buffer = unpacked;

			return false;
		}
		else
		{
			XLOG_CAT( "parse_dds", "    non-compressed bitmap." );
			size = w * h * li->block_bytes;
			return true;
		}
	}
}
