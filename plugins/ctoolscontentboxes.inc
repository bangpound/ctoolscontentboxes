<?php
// $Id$

/**
 * Base class for all ctools content types.
 */
abstract class ctoolscontentboxes extends boxes_box {
  public $content_type;
  public $plugin;

  /**
   * Implementation of boxes_content::__construct().
   */
  protected function __construct($content_type) {
    parent::__construct();
    ctools_include('content');
    $this->plugin = ctools_get_content_type($content_type);
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $defaults = array('subtype' => NULL);
    if (empty($this->plugin['defaults'])) {
      return $defaults;
    }
    return array_merge($this->plugin['defaults'], $defaults);
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form() {
    $plugin = $this->plugin;
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
    $plugin[$op .' form']($form, $form_state);

    // Add the form validator function as an element validator.
    if (function_exists('boxes_'. $plugin[$op .' form'] .'_validate')) {
      $form['#element_validate'][] = 'boxes_'. $plugin[$op .' form'] .'_validate';
    }
    return $form;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function render() {
    $type = $this->plugin['name'];
    $conf = $this->options;

    $content = ctools_content_render($type, $this->options['subtype'], $conf);
    $title = isset($this->title) ? check_plain($this->title) : NULL;
    return array(
      'delta' => $this->delta, // Crucial.
      'title' => $title,
      'subject' => $content->subject,
      'content' => $content->content,
    );
  }
}

