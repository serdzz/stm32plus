mw.loader.implement("ext.visualEditor.splitTest",function(){(function(mw,$){'use strict';function log(action){var dfd=$.Deferred();setTimeout(dfd.reject,1000);mw.loader.using('schema.Edit',function(){mw.eventLog.logEvent('Edit',{version:0,action:action,editor:'wikitext',pageId:mw.config.get('wgArticleId'),pageNs:mw.config.get('wgNamespaceNumber'),pageName:mw.config.get('wgPageName'),pageViewSessionId:mw.user.generateRandomSessionId(),revId:mw.config.get('wgCurRevisionId'),userId:+mw.config.get('wgUserId')}).always(dfd.resolve);});return dfd;}if(!mw.config.get('wgVisualEditorConfig',{}).enableEventLogging){return;}mw.hook('postEdit').add(function(){log('page-save-success');});if(mw.config.get('wgAction')==='edit'){log('page-edit-impression');}$('#ca-edit a, .mw-editsection a').on('click',function(e){var el=this,action=/section=/.test(el.href)?'section-edit-link-click':'edit-link-click';log(action).always(function(){window.location=el.href;});e.preventDefault();});$('#wpSave').one(
'click',function(e){var el=this;log('page-save-attempt').always(function(){$(el).trigger('click');});e.preventDefault();});}(mediaWiki,jQuery));;},{},{});
/* cache key: enwiki:resourceloader:filter:minify-js:7:508ce7f8df1a4a825ead831236f9e341 */