<?php
// $Id$

/**
 * Base class for all ctools content types.
 */
abstract class ctoolscontentboxes extends boxes_box {
  public $plugin;

  /**
   * Implementation of boxes_content::__construct().
   */
  protected function __construct($content_type) {
    ctools_include('content');
    $this->plugin = ctools_get_content_type($content_type);
    parent::__construct();
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $defaults = ctools_content_get_defaults($this->plugin, $this->options['subtype']);
    $defaults['subtype'] = NULL;
    return $defaults;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form() {
    $op = $this->new ? 'add' : 'edit';

    $form_state = array(
      'conf' => $this->options,
      'op' => $op,
    );

    $form = array();

    if ($this->new) {
      $types = ctools_content_get_subtypes($this->plugin['name']);
      if (count($types) > 1) {
        $options = array();
        foreach ($types as $key => $subtype) {
          $options[$subtype['category']][$key] = $subtype['title'];
        }
        if (count($options) > 1) {
          $form['subtype'] = array(
            '#type' => 'select',
            '#options' => $options,
            '#title' => 'Subtype',
          );
        }
      }
      else {
        $form['subtype'] = array(
          '#type' => 'value',
          '#value' => key($types),
        );
      }
    }
    else {
      $form['subtype'] = array(
        '#type' => 'value',
        '#value' => $this->options['subtype'],
      );
    }

    // Populate the form array.
    $this->plugin[$op .' form']($form, $form_state);

    // Add the form validator function as an element validator.
    if (function_exists('boxes_'. $this->plugin[$op .' form'] .'_validate')) {
      $form['#element_validate'][] = 'boxes_'. $this->plugin[$op .' form'] .'_validate';
    }
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $content = ctools_content_render($this->plugin['name'], $this->options['subtype'], $this->options);
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    return array(
      'delta' => $this->delta, // Crucial.
      'title' => $title,
      'subject' => $content->subject,
      'content' => $content->content,
    );
  }
}
