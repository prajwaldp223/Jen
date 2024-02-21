import React from 'react';
import WidgetContainer from './WidgetContainer';
import Stack from '@mui/material/Stack';
import Typography from '@mui/material/Typography';
import JenMenu from './JenMenu';
import JenSlider from './JenSlider';
import JenSwitch from './JenSwitch';

function WidgetGroup({ panelSize, json }) {
    const renderWidget = ( widget ) => {
      let widgetComponent;
      let height = 60;
  
      switch ( widget. type ) {
        case 'menu_int':
        case 'menu_string':
          widgetComponent = <JenMenu key = { widget.name } json = { widget } />;
          break;
        case 'slider_int':
        case 'slider_float':
        case 'range_slider_int':
        case 'range_slider_float':
          widgetComponent =       
            <Stack spacing={-0.5} direction="column" alignItems="center">
              <Typography style={{ textAlign: 'center' }}>
                  {widget.label}
              </Typography>
              <JenSlider key = { widget.name } json = { widget } width = { panelSize - 75 } />
            </Stack>;
          break;
        case 'switch_fn':
        case 'switch_condition':
          height = 30;
          widgetComponent =           
            <Stack spacing={1} direction="row" alignItems="center" sx={{ width: '100%', paddingLeft: '0px' }}>
              <JenSwitch key={widget.name} json={widget} size = { "small" } />
              <Typography variant="subtitle1" component="div">
                  {widget.label}
              </Typography>
            </Stack>;
          break;
          /*
          case 'widget_switch':
            // make height depend on switch state?
            widgetComponent =       
            <Stack spacing={-0.5} direction="column" alignItems="center">
              <Typography style={{ textAlign: 'center' }}>
                  {widget.label}
              </Typography>
              <JenSlider key = { widget.name } json = { widget } width = { panelSize - 75 } />
            </Stack>;
          break;
          */
        default:
          widgetComponent = <div key={ widget.name }>Unknown widget type: { widget.type }</div>;
          break;
      }
  
      return (
        <WidgetContainer key={ widget.name } panelSize = { panelSize } height = { height } >
          { widgetComponent }
        </WidgetContainer>
      );
    };
  
    return (
      <div>
        { json.widgets.map( renderWidget ) }
      </div>
    );
}
  
export default WidgetGroup;
