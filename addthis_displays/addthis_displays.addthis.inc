<?php

/**
 * @file
 * Implements AddThis markup functions.
 */

function addthis_displays_addthis_display_markup($display, $variables = NULL) {
  return NULL;
}

/**
 * Implements hook_addthis_display_markup__[display]().
 */
function addthis_displays_addthis_display_markup__addthis_basic_toolbox($options = array()) {
  $addthis = AddThis::getInstance();

  // Create a render array for the widget.
  $element = array(
    // Use #theme_wrappers to include the rendered children. Otherwise the
    // result is an empty element like <div></div>.
    '#theme' => 'addthis_wrapper',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array(
        'addthis_toolbox',
        'addthis_default_style',
        ($options['#display']['settings']['buttons_size'] == AddThis::CSS_32x32 ? AddThis::CSS_32x32 : NULL),
        $options['#display']['settings']['extra_css'],
      ),
    ),
  );
  $element['#attributes'] += $addthis->getAddThisAttributesMarkup($options);

  $services = trim($options['#display']['settings']['share_services']);
  $services = str_replace(' ', '', $services);
  $services = explode(',', $services);

  // All service elements
  $items = array();
  foreach ($services as $service) {
    $items[$service] = array(
      '#theme' => 'addthis_element',
      '#tag' => 'a',
      '#value' => '',
      '#attributes' => array(
        'href' => $addthis->getBaseBookmarkUrl(),
        'class' => array(
          'addthis_button_' . $service,
        ),
      ),
      '#addthis_service' => $service,
    );

    // Basic implementations of bubble counter orientation.
    // @todo Figure all the bubbles out and add them.
    //   Still missing: tweetme, hyves and stubleupon.
    //
    // @todo Fix a way to use addthis_bubble_style.
    //   There is a conflict now with using the class addthis_button_[service].
    //   You can't add this bubble style now.
    // $options['#display']['settings']['buttons_size']
    $buttonbig = ($options['#display']['settings']['buttons_size'] == AddThis::CSS_32x32 ? TRUE : FALSE);
    $bubblecounts = ($options['#display']['settings']['counter_enabled'] == 'true' ? TRUE : FALSE);
    $orientation = ($options['#display']['settings']['counter_orientation'] == 'horizontal' ? TRUE : FALSE);
    switch ($service) {
      case 'addthis':
        $items[$service]['#attributes'] = array(
            'class' => array(
              'addthis_counter addthis_pill_style'
            )
        );
        break;  
      case 'facebook_like':
        $items[$service]['#attributes'] += array(
          'fb:like:layout' => ($orientation ? 'button_count' : 'box_count')
        );
        break;
      case 'google_plusone':
        //TODO Integrate annotation & size options.
        $items[$service]['#attributes'] += array(
          'g:plusone:size' => ($buttonbig ? 'tall' : 'medium'),
          //('small' : 'medium' : 'standard' : 'tall')
          'g:plusone:annotation' => ($bubblecounts ? 'bubble' : 'none')
          //( 'none' : 'inline' : 'bubble')
        );
        break;
      case 'tweet':
        $items[$service]['#attributes'] += array(
          //TODO Integrate count options.
          'tw:count' => ($bubblecounts ? ($orientation ? 'horizontal' : 'vertical') : 'none'),
          //($orientation ? 'none' : 'horizontal' : 'vertical'),
          'tw:via' => AddThis::getInstance()->getTwitterVia(),
        );
        break;
    }
  }
  $element += $items;

  return $element;
}

/**
 * Implements hook_addthis_display_markup__[display]().
 */
function addthis_displays_addthis_display_markup__addthis_basic_button($options = array()) {

  $button_img = 'http://s7.addthis.com/static/btn/sm-share-en.gif';
  if ($options['#display']['settings']['button_size'] == 'big') {
    $button_img = 'http://s7.addthis.com/static/btn/v2/lg-share-en.gif';
  }

  $element = array(
    '#theme' => 'addthis_wrapper',
    '#tag' => 'a',
    '#attributes' => array(
      'class' => array(
        'addthis_button',
        $options['#display']['settings']['extra_css'],
      ),
    ),
  );
  $element['#attributes'] += AddThis::getInstance()->getAddThisAttributesMarkup($options);

  // Create img button
  $image = array(
    '#theme' => 'addthis_element',
    '#tag' => 'img',
    '#attributes' => array(
      'src' => $button_img,
      'alt' => t('Share page with AddThis'),
    ),
  );
  $element[] = $image;

  return $element;
}
