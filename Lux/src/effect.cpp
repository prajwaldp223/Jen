
#include "effect.hpp"
#include "scene.hpp"
#include "life.hpp"

void eff_identity::operator () ( any_buffer_pair_ptr &buf, element_context& context ) {} // Does nothing

// Fill image with color
template< class T > void eff_fill< T >::operator () ( any_buffer_pair_ptr& buf, element_context& context )  { 
    //std::cout << "eff_fill: " << std::endl;
    fill_color( context ); 
    bounds( context );
    if (std::holds_alternative< std::shared_ptr< buffer_pair< T > > >(buf))
    {
        auto& buf_ptr = std::get< std::shared_ptr< buffer_pair< T > > >(buf);
        if( !buf_ptr->has_image() ) throw std::runtime_error( "eff_fill: no image in buffer" );
        if( bounded ) buf_ptr->get_image().fill( *fill_color, *bounds );
        else          buf_ptr->get_image().fill( *fill_color );
    }
    // else convert between pixel types
}

template class eff_fill< frgb >;
template class eff_fill< ucolor >;
template class eff_fill< vec2f >;
template class eff_fill< int >;
template class eff_fill< vec2i >;

// Noise effect
template< class T > void eff_noise< T >::operator () ( any_buffer_pair_ptr& buf, element_context& context )  { 
    bounds( context ); 
    a( context );
    if (std::holds_alternative< std::shared_ptr< buffer_pair< T > > >(buf))
    {
        auto& buf_ptr = std::get< std::shared_ptr< buffer_pair< T > > >(buf);
        if( !buf_ptr->has_image() ) throw std::runtime_error( "eff_fill: no image in buffer" );
        if( bounded ) buf_ptr->get_image().noise( *a, *bounds );
        else          buf_ptr->get_image().noise( *a );
    }
    // else convert between pixel types
}

template class eff_noise< frgb >;
template class eff_noise< ucolor >;
template class eff_noise< vec2f >;
template class eff_noise< int >;
template class eff_noise< vec2i >;

template< class T > void eff_vector_warp< T >::operator () ( any_buffer_pair_ptr& buf, element_context& context )  { 
    if (std::holds_alternative< std::shared_ptr< buffer_pair< T > > >(buf))
    {
        auto& buf_ptr = std::get< std::shared_ptr< buffer_pair< T > > >(buf);
        if( !buf_ptr->has_image() ) throw std::runtime_error( "eff_vector_warp: no image in buffer" );
        any_buffer_pair_ptr vf_buf = context.s.buffers[ vf_name ];
        auto& vf = std::get< vbuf_ptr >( vf_buf )->get_image();   // extract vector field from buffer variant
        buf_ptr->get_buffer().warp( buf_ptr->get_image(), vf, *step, smooth, relative, extend );
        buf_ptr->swap();
    }
}

template class eff_vector_warp< frgb >;
template class eff_vector_warp< ucolor >;
template class eff_vector_warp< vec2f >;
template class eff_vector_warp< int >;
template class eff_vector_warp< vec2i >;

template< class T > void eff_feedback< T >::operator () ( any_buffer_pair_ptr& buf, element_context& context )
{
    if (std::holds_alternative< std::shared_ptr< buffer_pair< T > > >(buf))
    {
        auto& buf_ptr = std::get< std::shared_ptr< buffer_pair< T > > >(buf);
        if( !buf_ptr->has_image() ) throw std::runtime_error( "eff_feedback: no image in buffer" );
        any_buffer_pair_ptr wf_buf = context.s.buffers[ wf_name ];
        auto& wf = std::get< wbuf_ptr >( wf_buf )->get_image();  // extract warp field from buffer variant
        buf_ptr->get_image().warp( buf_ptr->get_image(), wf );
        buf_ptr->swap();
    }
}

template class eff_feedback< frgb >;
template class eff_feedback< ucolor >;
template class eff_feedback< vec2f >;
template class eff_feedback< int >;
template class eff_feedback< vec2i >;

void eff_n::operator () ( any_buffer_pair_ptr &buf, element_context& context )
{
    n( context );
    for( int i = 0; i < *n; i++ ) eff( buf, context );
}

void eff_n::set_effect( any_effect_fn& eff_init ) { eff = eff_init; }

//void eff_n( int& n_init, any_effect_fn& eff_init ) : n( n_init ), eff( eff_init ) {}

// Composite effect - runs a list of component effects
void eff_composite::operator () ( any_buffer_pair_ptr& buf, element_context& context )  {
    for( auto eff : effects ) { eff( buf, context );  }
}

void eff_composite::add_effect( const any_effect_fn& eff ) { effects.push_back( eff ); }




